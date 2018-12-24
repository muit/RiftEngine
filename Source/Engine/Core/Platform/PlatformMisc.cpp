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

	DateTime EstimatedCurrentDateTime;

	if (IncrementCounter == 0)
	{
		InitialDateTime = DateTime::Now();
		EstimatedCurrentDateTime = InitialDateTime;
	}
	else
	{
		EstimatedCurrentDateTime = DateTime::Now();
	}

	uint32 SequentialBits = static_cast<uint32>(IncrementCounter++); // Add sequential bits to ensure sequentially generated guids are unique even if Cycles is wrong
	uint32 RandBits = Math::Rand() & 0xFFFF; // Add randomness to improve uniqueness across machines

	guid = Guid(RandBits | (SequentialBits << 16), EstimatedCurrentDateTime.GetTicks() >> 32, EstimatedCurrentDateTime.GetTicks() & 0xffffffff, PlatformTime::Cycles());
}
