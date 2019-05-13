// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"

#include <EABase/eabase.h>
#include <EASTL/allocator_malloc.h>
#include <EASTL/string_view.h>
#include <tracy/Tracy.hpp>
#include <tracy/client/tracy_rpmalloc.hpp>
#include <new>
#include <foundation/PxAllocatorCallback.h>


///////////////////////////////////////////////////////////////////////////////
// Allocator
//
class Allocator
{
	const TCHAR* name;
	size_t size;
	eastl::allocator_malloc malloc_alloc;

public:
	Allocator(const TCHAR* name = TX("Global"));
	Allocator(const Allocator&);
	Allocator(const Allocator&, const TCHAR* name);

	Allocator& operator=(const Allocator&) { return *this; }

	bool operator==(const Allocator&){ return true; }

	bool operator!=(const Allocator&) { return false; }

	void* Allocate(size_t n, int flags = 0)
	{
		//size += n;
		void* const ptr = malloc_alloc.allocate(n, flags);
		TracyAllocS(ptr, n, 8);
		return ptr;
	}

	void* Allocate(size_t n, size_t alignment, size_t alignmentOffset, int flags = 0)
	{
		//size += n;
		void* const ptr = malloc_alloc.allocate(n, alignment, alignmentOffset, flags);
		TracyAllocS(ptr, n, 8);
		return ptr;
	}

	void Deallocate(void* const ptr)
	{
		// #TODO: obtain size of the ptr
		TracyFreeS(ptr, 8);
		malloc_alloc.deallocate(ptr, 0);
	}

	void Deallocate(void* const ptr, size_t n)
	{
		//size -= n;
		TracyFreeS(ptr, 8);
		malloc_alloc.deallocate(ptr, n);
	}

	struct Name GetName() const;

	void SetName(const TCHAR* Name);

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
	using Super = eastl::allocator;

	const TCHAR* name;

public:

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

	const TCHAR* GetName() const { return name; }
	void SetName(const TCHAR* inName) { name = inName; }
};


class PhysicsAllocator : public physx::PxAllocatorCallback
{
	const TCHAR* name;


public:
	PhysicsAllocator(const TCHAR* name = TX("PhysX")) : name{ name } {}

	virtual void* allocate(size_t size, const char*, const char*, int) override
	{
#if PX_WINDOWS_FAMILY
		void* const ptr = _aligned_malloc(size, 16);
#elif PX_LINUX_FAMILY || PX_SWITCH
		void* const ptr = ::memalign(16, size);
#else
		void* const ptr = ::malloc(size);
#endif
		TracyAllocS(ptr, size, 8);
		return ptr;
	}

	virtual void deallocate(void* ptr) override
	{
		TracyFreeS(ptr, 8);
#if PX_WINDOWS_FAMILY
		_aligned_free(ptr);
#elif PX_LINUX_FAMILY || PX_SWITCH
		::free(ptr);
#else
		::free(ptr);
#endif
	}

	const TCHAR* GetName() const { return name; }
	void SetName(const TCHAR* inName) { name = inName; }
};


namespace Memory {
	EASTL_API PhysicsAllocator* GetPhysicsAllocator();
}
