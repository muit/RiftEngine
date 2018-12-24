// Copyright 2015-2019 Piperift - All rights reserved

#include "PlatformMisc.h"
#include "Core/Misc/Guid.h"
#include "Core/Misc/DateTime.h"
#include "Core/Misc/Timespan.h"
#include "PlatformTime.h"


void PlatformMisc::CreateGuid(Guid& guid)
{
	static uint16 IncrementCounter = 0;

	static DateTime InitialDateTime;
	static uint64 InitialCycleCounter;

	DateTime EstimatedCurrentDateTime;

	if (IncrementCounter == 0)
	{
		// Hack: First Guid can be created prior to FPlatformTime::InitTiming(), so do it here.
		//PlatformTime::InitTiming();

		// uses FPlatformTime::SystemTime()
		InitialDateTime = DateTime::Now();
		InitialCycleCounter = PlatformTime::Cycles64();

		EstimatedCurrentDateTime = InitialDateTime;
	}
	else
	{
		Timespan ElapsedTime = Timespan::FromSeconds(PlatformTime::ToSeconds64(PlatformTime::Cycles64() - InitialCycleCounter));

		EstimatedCurrentDateTime = InitialDateTime + ElapsedTime;
	}

	uint32 SequentialBits = static_cast<uint32>(IncrementCounter++); // Add sequential bits to ensure sequentially generated guids are unique even if Cycles is wrong
	uint32 RandBits = Math::Rand() & 0xFFFF; // Add randomness to improve uniqueness across machines

	guid = Guid(RandBits | (SequentialBits << 16), EstimatedCurrentDateTime.GetTicks() >> 32, EstimatedCurrentDateTime.GetTicks() & 0xffffffff, PlatformTime::Cycles());
}
