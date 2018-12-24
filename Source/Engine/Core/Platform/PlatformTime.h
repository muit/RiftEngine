// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreEngine.h"


struct PlatformTime
{
	static void SystemTime(int32& year, int32& month, int32& dayOfWeek, int32& day, int32& hour, int32& min, int32& sec, int32& mSec);
	static void UtcTime(int32& year, int32& month, int32& dayOfWeek, int32& day, int32& hour, int32& min, int32& sec, int32& mSec);
};
