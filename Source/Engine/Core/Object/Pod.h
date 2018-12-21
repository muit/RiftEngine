// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"
#include <EASTL/type_traits.h>

#include "../Memory/Allocator.h"


class Pod {

public:

	Pod() = default;
	virtual ~Pod() {}

	void* operator new(size_t size) { return Memory::GetObjectsAllocator()->Allocate(size); }
	void operator delete(void* p, std::size_t size) { Memory::GetObjectsAllocator()->Deallocate(p, size); }
};
