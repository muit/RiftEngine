// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EABase/eabase.h>
#include <EASTL/allocator_malloc.h>
#include <EASTL/string_view.h>
#include <new>

#include "Util/Name.h"


///////////////////////////////////////////////////////////////////////////////
// Allocator
//
class Allocator
{
	Name name;
	size_t size;
	eastl::allocator_malloc malloc_alloc;

public:
	Allocator(const char* name = "Global")        : name{ name }, size{0}, malloc_alloc{ name } {}
	Allocator(const Allocator&)                   : name{},       size{0}, malloc_alloc{}       {}
	Allocator(const Allocator&, const char* name) : name{ name }, size{0}, malloc_alloc{ name } {}

	Allocator& operator=(const Allocator&) { return *this; }

	bool operator==(const Allocator&){ return true; }

	bool operator!=(const Allocator&) { return false; }

	void* Allocate(size_t n, int flags = 0)
	{
		size += n;
		return malloc_alloc.allocate(n, flags);
	}

	void* Allocate(size_t n, size_t alignment, size_t alignmentOffset, int flags = 0)
	{
		size += n;
		return malloc_alloc.allocate(n, alignment, alignmentOffset, flags);
	}

	void Deallocate(void* p, size_t n)
	{
		size -= n;
		malloc_alloc.deallocate(p, n);
	}

	Name GetName() const
	{
		return name;
	}

	void SetName(const char* inName) { name = inName; }

	size_t GetSize() const
	{
		return size;
	}
};


namespace FecoEngine {
	EASTL_API Allocator* GetAllocator();

	EASTL_API Allocator* GetObjectsAllocator();

	EASTL_API Allocator* GetAssetsAllocator();

	EASTL_API Allocator* GetFrameAllocator();
}
