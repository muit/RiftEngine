// Copyright 2015-2019 Piperift - All rights reserved

#include "Allocator.h"

namespace FecoEngine {
	/// gAllocator
	/// Default engine allocator instance.
	EASTL_API Allocator   gAllocator {};
	EASTL_API Allocator* gpAllocator = &gAllocator;

	EASTL_API Allocator* GetAllocator()
	{
		return gpAllocator;
	}

	/// gAllocator
	/// Default engine allocator instance.
	EASTL_API Allocator   gObjectsAllocator{ "Objects" };
	EASTL_API Allocator* gpObjectsAllocator = &gObjectsAllocator;

	EASTL_API Allocator* GetObjectsAllocator()
	{
		return gpObjectsAllocator;
	}

	/// gAllocator
	/// Default engine allocator instance.
	EASTL_API Allocator   gAssetsAllocator{"Assets"};
	EASTL_API Allocator* gpAssetsAllocator = &gAssetsAllocator;

	EASTL_API Allocator* GetAssetsAllocator()
	{
		return gpAssetsAllocator;
	}

	/// gAllocator
	/// Default engine allocator instance.
	EASTL_API Allocator   gFrameAllocator{"Frame"};
	EASTL_API Allocator* gpFrameAllocator = &gFrameAllocator;

	EASTL_API Allocator* GetFrameAllocator()
	{
		return gpFrameAllocator;
	}
};


// EASTL News
void* operator new[](size_t size, const char* /*name*/, int flags,
	unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
	return FecoEngine::GetAllocator()->Allocate(size, flags);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* /*name*/,
	int flags, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
	return FecoEngine::GetAllocator()->Allocate(size, alignment, alignmentOffset, flags);
}


// Native News
void* operator new(size_t size)
{
	return FecoEngine::GetAllocator()->Allocate(size);
}

void* operator new[](size_t size)
{
	return FecoEngine::GetAllocator()->Allocate(size);
}


// Deletes
void operator delete(void* p, std::size_t size)
{
	FecoEngine::GetAllocator()->Deallocate(p, size);
}

void operator delete[](void* p, std::size_t size)
{
	FecoEngine::GetAllocator()->Deallocate(p, size);
}

