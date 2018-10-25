// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/type_traits.h>

#include "EngineTypes.h"
#include "Reflection/Reflection.h"
#include "Memory/Allocator.h"


class Class;
class Object;

template<typename Type>
class Ptr;


class BaseObject {

public:

	BaseObject() = default;
	virtual ~BaseObject() {}

	void Destroy() { BeforeDestroy(); }

	Class* GetClass() const;
	Ptr<Object> GetSelf() const;


	void* operator new(size_t size) { return Memory::GetObjectsAllocator()->Allocate(size); }
	void operator delete(void* p, std::size_t size) { Memory::GetObjectsAllocator()->Deallocate(p, size); }

protected:

	virtual void BeforeDestroy() {}
};
