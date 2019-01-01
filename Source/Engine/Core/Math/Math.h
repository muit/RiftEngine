// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"
#include <cmath>


struct Math
{
	static constexpr float PI{ 3.14159265358979323846f };
	static constexpr float SMALL_NUMBER{ 1.e-4f };
	static constexpr float SMALLER_NUMBER{ 0.00001f };
	static constexpr float VERY_SMALL_NUMBER{ 1.e-8f };
	static constexpr float BIG_NUMBER{ 3.4e+38f };
	static constexpr float EULER{ 2.71828182845904523536f };


	template<typename Type>
	static FORCEINLINE constexpr Type Max(Type a, Type b) { return a >= b ? a : b; }

	template<typename Type>
	static FORCEINLINE constexpr Type Min(Type a, Type b) { return a <= b ? a : b; }

	template<typename Type>
	static FORCEINLINE constexpr Type Clamp(Type a, Type min, Type max) { return Max(min, Min(a, max)); }

	/** Computes absolute value in a generic way */
	template<typename Type>
	static FORCEINLINE constexpr Type Abs(const Type a)
	{
		return (a >= (Type)0) ? a : -a;
	}

	/** Returns 1, 0, or -1 depending on relation of Type to 0 */
	template<typename Type>
	static FORCEINLINE constexpr Type Sign(const Type A)
	{
		return (a > (Type)0) ? (Type)1 : ((a < (Type)0) ? (Type)-1 : (Type)0);
	}


	/** Returns a random integer between 0 and RAND_MAX, inclusive */
	static FORCEINLINE int32 Rand() { return rand(); }


	/**
	* Converts a floating point number to an integer which is further from zero, "larger" in absolute value: 0.1 becomes 1, -0.1 becomes -1
	* @param F		Floating point value to convert
	* @return		The rounded integer
	*/
	static FORCEINLINE float RoundFromZero(float F)
	{
		return (F < 0.0f) ? FloorToFloat(F) : CeilToFloat(F);
	}

	static FORCEINLINE double RoundFromZero(double F)
	{
		return (F < 0.0) ? FloorToDouble(F) : CeilToDouble(F);
	}

	/**
	* Converts a floating point number to an integer which is closer to zero, "smaller" in absolute value: 0.1 becomes 0, -0.1 becomes 0
	* @param F		Floating point value to convert
	* @return		The rounded integer
	*/
	static FORCEINLINE float RoundToZero(float f)
	{
		return (f < 0.0f) ? CeilToFloat(f) : FloorToFloat(f);
	}

	static FORCEINLINE double RoundToZero(double d)
	{
		return (d < 0.0) ? CeilToDouble(d) : FloorToDouble(d);
	}

	/**
	* Converts a floating point number to an integer which is more negative: 0.1 becomes 0, -0.1 becomes -1
	* @param F		Floating point value to convert
	* @return		The rounded integer
	*/
	static FORCEINLINE float RoundToNegativeInfinity(float f)
	{
		return FloorToFloat(f);
	}

	static FORCEINLINE double RoundToNegativeInfinity(double d)
	{
		return FloorToDouble(d);
	}

	/**
	* Converts a floating point number to an integer which is more positive: 0.1 becomes 1, -0.1 becomes 0
	* @param F		Floating point value to convert
	* @return		The rounded integer
	*/
	static FORCEINLINE float RoundToPositiveInfinity(float f)
	{
		return CeilToFloat(f);
	}

	static FORCEINLINE double RoundToPositiveInfinity(double d)
	{
		return CeilToDouble(d);
	}

	static FORCEINLINE int32 FloorToInt(float f)
	{
		return (int32)FloorToFloat(f);
	}

	static FORCEINLINE float FloorToFloat(float f)
	{
		return std::floor(f);
	}

	static FORCEINLINE double FloorToDouble(double d)
	{
		return std::floor(d);
	}

	static FORCEINLINE int32 CeilToInt(float f)
	{
		return (int32)CeilToFloat(f);
	}

	static FORCEINLINE float CeilToFloat(float f)
	{
		return std::ceil(f);
	}

	static FORCEINLINE double CeilToDouble(double d)
	{
		return std::ceil(d);
	}

	template<class T>
	static FORCEINLINE T Sqrt(T val) {
		return std::sqrt(val);
	}

	template<class T>
	static FORCEINLINE T Square(T val) {
		return val * val;
	}
};
