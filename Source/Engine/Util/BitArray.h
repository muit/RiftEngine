// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Platform/Platform.h"


class BitArray
{
private:
	/** The number of bits in this array */
	uint32 arraySize;

	/** The number of uint32s for storing at least arraySize bits */
	uint32 bitLongs;

	/** The array of uint32s containing the bits */
	uint32 *bits;

public:

	/** Constructors */
	BitArray(uint32 newSize);
	BitArray(BitArray&& other);
	BitArray(const BitArray& other);

	BitArray(uint32 newSize, bool setClear);
	BitArray(uint32 newSize, uint32 *newBits);

	/** Destructor */
	~BitArray();

	BitArray& operator=(BitArray&& other);
	BitArray& operator=(const BitArray& other);
	BitArray  operator~();
	BitArray& operator^=(const BitArray& other);
	BitArray& operator&=(const BitArray& other);
	BitArray& operator|=(const BitArray& other);
	BitArray  operator^(const BitArray& other);
	BitArray  operator&(const BitArray& other);
	BitArray  operator|(const BitArray& other);

	/** @return true if a bit is set */
	inline bool IsValidIndex(uint32 index) const
	{
		return bits[(index>>5)] >> (index & 0x0000001f) & 0x00000001;
	}

	/** Set all bits in this array */
	inline void FillBits()
	{
		FillBitArray(0xffffffff);
	}

	/** Set a single bit */
	inline void FillBit(uint32 index)
	{
		bits[index >> 5] |= 0x00000001 << (index & 0x0000001f);
	}

	/** Clear all bits in this array */
	inline void ClearBits()
	{
		FillBitArray(0x00000000);
	}

	/** Clear a single bit */
	inline void ClearBit(uint32 index)
	{
		bits[index >> 5] &= ~(0x00000001 << (index & 0x0000001f));
	}
	/** Fill with a 32-bit pattern */
	void FillBitArray(uint32 pattern);

	/** flip a single bit.
	*/
	inline void FlipBit(uint32 index)
	{
		if (IsValidIndex(index))
			ClearBit(index);
		else
			FillBit(index);
	};

	inline void Clear() {
		delete[]bits;
		arraySize = 0;
		bitLongs = 0;
		bits = nullptr;
	}

	/** Returns index of next set bit in array (wraps around)
	*/
	int32 GetNextSet(uint32 index) const;

	/** @return index of previous set bit in array (wraps around) */
	int32 GetPreviousSet(uint32 index) const;

	/** @return the number of bits in this bit array */
	uint32 Size() { return arraySize; }
};
