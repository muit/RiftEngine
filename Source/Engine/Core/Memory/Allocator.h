// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EABase/eabase.h>
#include <EASTL/allocator_malloc.h>
#include <EASTL/string_view.h>
#include <tracy/Tracy.hpp>
#include <new>


///////////////////////////////////////////////////////////////////////////////
// Allocator
//
class Allocator
{
	const char* name;
	size_t size;
	eastl::allocator_malloc malloc_alloc;

public:
	Allocator(const char* name = "Global");
	Allocator(const Allocator&);
	Allocator(const Allocator&, const char* name);

	Allocator& operator=(const Allocator&) { return *this; }

	bool operator==(const Allocator&){ return true; }

	bool operator!=(const Allocator&) { return false; }

	void* Allocate(size_t n, int flags = 0)
	{
		size += n;
		void* const ptr = malloc_alloc.allocate(n, flags);
		TracyAllocS(ptr, n, 10);
		return ptr;
	}

	void* Allocate(size_t n, size_t alignment, size_t alignmentOffset, int flags = 0)
	{
		size += n;
		void* const ptr = malloc_alloc.allocate(n, alignment, alignmentOffset, flags);
		TracyAllocS(ptr, n, 10);
		return ptr;
	}

	void Deallocate(void* const ptr, size_t n)
	{
		size -= n;
		TracyFreeS(ptr, 10);
		malloc_alloc.deallocate(ptr, n);
	}

	struct Name GetName() const;

	void SetName(const char* Name);

	size_t GetSize() const
	{
		return size;
	}
};


namespace Memory {
	EASTL_API Allocator* GetAllocator();

	EASTL_API Allocator* GetObjectsAllocator();

	EASTL_API Allocator* GetAssetsAllocator();

	EASTL_API Allocator* GetFrameAllocator();
}


class StringAllocator : public eastl::allocator
{
public:
	using Super = eastl::allocator;

	EASTL_ALLOCATOR_EXPLICIT StringAllocator(const char* pName = EASTL_NAME_VAL(EASTL_ALLOCATOR_DEFAULT_NAME)) : Super(pName) {}

	void* allocate(size_t n, int flags = 0)
	{
		return Memory::GetAllocator()->Allocate(n, flags);
	}

	void* allocate(size_t n, size_t alignment, size_t offset, int flags = 0)
	{
		return Memory::GetAllocator()->Allocate(n, alignment, offset, flags);
	}

	void deallocate(void* p, size_t n)
	{
		Memory::GetAllocator()->Deallocate(p, n);
	}

	const char* GetName() const { return Super::get_name(); }
	void SetName(const char* pName) { Super::set_name(pName); }
};
