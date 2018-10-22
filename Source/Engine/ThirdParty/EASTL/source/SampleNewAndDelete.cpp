/////////////////////////////////////////////////////////////////////////////
// Copyright (c) Electronic Arts Inc. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include <EABase/eabase.h>
#include <stddef.h>
#include <new>

void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return new uint8_t[size];
}

void* operator new[](size_t size, size_t s2, size_t s3, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return new uint8_t[size];
}
