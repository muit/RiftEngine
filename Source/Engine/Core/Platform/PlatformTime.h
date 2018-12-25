// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <chrono>

#include "Core/Misc/DateTime.h"


struct PlatformTime
{
	static FORCEINLINE uint32 Cycles() {
		return (uint32)Cycles64();
	}
	static FORCEINLINE uint64 Cycles64() {
		return floor<std::chrono::microseconds>(DateTime::Now().GetTime().time_since_epoch()).count();
	}
};
