// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <assert.h>
#include <EASTL/vector.h>
#include <EASTL/functional.h>

#include "Core/Platform/Platform.h"


template<typename Type, typename Allocator = EASTLAllocatorType>
class TArray {
public:
	template <typename OtherType, typename OtherAllocator>
	friend class TArray;

	using VectorType = eastl::vector<Type, Allocator>;

	using Iterator = typename VectorType::iterator;
	typedef typename VectorType::const_iterator         ConstIterator;
	typedef typename VectorType::reverse_iterator       ReverseIterator;
	typedef typename VectorType::const_reverse_iterator ConstReverseIterator;
	

private:

	VectorType vector;

public:

	TArray() : vector{} {}

	TArray(std::initializer_list<Type> initList) : vector{initList} {}

	TArray(TArray<Type>&& other) { MoveFrom(eastl::move(other)); }
	TArray<Type>& operator=(TArray<Type>&& other) {
		MoveFrom(eastl::move(other));
		return *this;
	}

	TArray(const TArray<Type>& other) { CopyFrom(other); }
	TArray<Type>& operator=(const TArray<Type>& other) {
		CopyFrom(other);
		return *this;
	}

	void Reserve(int32 sizeNum) { vector.reserve(sizeNum); }

	int32 Add(Type&& item) {
		vector.push_back(eastl::move(item));
		return Size() - 1;
	}

	int32 Add(const Type& item) {
		vector.push_back(item);
		return Size() - 1;
	}

	int32 AddDefaulted() {
		vector.push_back();
		return Size() - 1;
	}

	void Insert(int32 index, Type&& item) {
		vector.insert(vector.begin() + index, eastl::move(item));
	}

	void Insert(int32 index, const Type& item) {
		vector.insert(vector.begin() + index, item);
	}

	void InsertDefaulted(int32 index, int32 count) {
		if (IsValidIndex(index))
		{
			vector.reserve(vector.size() + count);
			for (int32 i = 0; i < count; ++i)
			{
				Insert(index + i, {});
			}
		}
	}

	FORCEINLINE ConstIterator FindIt(const Type& item) const {
		return eastl::find_if(vector.begin(), vector.end(), item);
	}
	int32 FindIndex(const Type& item) const {
		ConstIterator found = FindIt(item);
		if (found != vector.end())
		{
			return eastl::distance(vector.begin(), found);
		}
		return NoIndex;
	}

	/**
	 * Delete all items that match another provided item
	 * @return number of deleted items
	 */
	int32 Remove(const Type& item) {
		const int32 lastSize = Size();
		eastl::remove(vector.begin(), vector.end(), item);
		return lastSize - Size();
	}

	/**
	 * Delete item at index
	 * @return true if removed
	 */
	bool RemoveAt(int32 index) {
		if (IsValidIndex(index))
		{
			const int32 lastSize = Size();
			vector.erase(vector.begin() + index);
			return lastSize - Size() > 0;
		}
		return false;
	}

	/**
	 * Delete item at index. Doesn't preserve order but its considerably faster
	 * @return true if removed
	 */
	bool RemoveAtSwap(int32 index) {
		if (IsValidIndex(index))
		{
			const int32 lastSize = Size();
			Swap(index, lastSize - 1)
			vector.pop_back();
			return lastSize - Size() > 0;
		}
		return false;
	}

	/**
	 * Delete all items that match a delegate
	 * @return number of deleted items
	 */
	int32 RemoveIf(eastl::function<bool(const Type&)>&& callback) {
		int32 lastSize = Size();
		vector.erase(eastl::remove_if(v.begin(), v.end(), callback), v.end());
		return lastSize - Size();
	}

	void Swap(int32 firstIndex, int32 secondIndex) {
		if (size() > 1 && firstIndex != secondIndex && IsValidIndex(firstIndex) && IsValidIndex(secondIndex))
		{
			eastl::swap(vector.begin() + firstIndex, vector.begin() + secondIndex);
		}
	}

	void Empty() { vector.empty(); }

	FORCEINLINE int32 Size() const { return vector.size(); }

	FORCEINLINE bool IsValidIndex(int32 Index) const
	{
		return Index >= 0 && Index < Size();
	}

	/** OPERATORS */
public:

	/**
	 * Array bracket operator. Returns reference to element at give index.
	 *
	 * @returns Reference to indexed element.
	 */
	FORCEINLINE Type& operator[](int32 index)
	{
		assert(IsValidIndex(index));
		return vector.at(index);
	}

	/**
	 * Array bracket operator. Returns reference to element at give index.
	 *
	 * Const version of the above.
	 *
	 * @returns Reference to indexed element.
	 */
	FORCEINLINE const Type& operator[](int32 index) const
	{
		assert(IsValidIndex(index));
		return vector.at(index);
	}

	FORCEINLINE Iterator      begin()        { return vector.begin();  };
	FORCEINLINE ConstIterator begin()  const { return vector.begin();  };
	FORCEINLINE ConstIterator cbegin() const { return vector.cbegin(); };

	FORCEINLINE Iterator      end()        { return vector.end();  };
	FORCEINLINE ConstIterator end()  const { return vector.end();  };
	FORCEINLINE ConstIterator cend() const { return vector.cend(); };

	FORCEINLINE ReverseIterator      rbegin()        { return vector.rbegin();  };
	FORCEINLINE ConstReverseIterator rbegin()  const { return vector.rbegin();  };
	FORCEINLINE ConstReverseIterator crbegin() const { return vector.crbegin(); };

	FORCEINLINE ReverseIterator      rend()        { return vector.rend();  };
	FORCEINLINE ConstReverseIterator rend()  const { return vector.rend();  };
	FORCEINLINE ConstReverseIterator crend() const { return vector.crend(); };


	/** INTERNAl */
private:

	FORCEINLINE void CopyFrom(const TArray& other) { vector = other.vector; }
	FORCEINLINE void MoveFrom(TArray&& other) { vector = eastl::move(other.vector); }


	/** STATIC */
public:

	static constexpr int32 NoIndex = -1;
};
