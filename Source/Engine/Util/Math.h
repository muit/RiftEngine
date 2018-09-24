// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "EngineTypes.h"


namespace Math
{
	template<typename Type>
	Type Min(Type a, Type min) { return a < min ? a : min; }
	template<typename Type>
	Type Max(Type a, Type max) { return a > max ? a : max; }

	template<typename Type>
	Type Clamp(Type a, Type min, Type max) { return Max(min, Min(a, max)); }
};
