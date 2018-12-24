// Copyright 2015-2019 Piperift - All rights reserved

#include "PlatformTime.h"
#include <EASTL/chrono.h>


void PlatformTime::SystemTime(int32& year, int32& month, int32& dayOfWeek, int32& day, int32& hour, int32& min, int32& sec, int32& mSec)
{
	auto now = eastl::chrono::steady_clock::now();
	// #TODO: Use Date library
}

void PlatformTime::UtcTime(int32& year, int32& month, int32& dayOfWeek, int32& day, int32& hour, int32& min, int32& sec, int32& mSec)
{

}
