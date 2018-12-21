// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"
#include <cmath>


namespace Math
{
	constexpr float PI { 3.14159265358979323846f };

	template<typename Type>
	FORCEINLINE constexpr Type Min(Type a, Type min) { return a < min ? a : min; }
	template<typename Type>
	FORCEINLINE constexpr Type Max(Type a, Type max) { return a > max ? a : max; }

	template<typename Type>
	FORCEINLINE constexpr Type Clamp(Type a, Type min, Type max) { return Max(min, Min(a, max)); }
};
