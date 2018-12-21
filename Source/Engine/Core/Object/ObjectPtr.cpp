// Copyright 2015-2019 Piperift - All rights reserved

#include "ObjectPtr.h"
#include "Object.h"


void BaseGlobalPtr::MoveFrom(BaseGlobalPtr&& other)
{
	ptr = eastl::move(other.ptr);
	weaks = eastl::move(other.weaks);

	// Update owned pointers to owner
	for (BaseWeakPtr* const weak : weaks)
	{
		weak->globalPtr = this;
	}
}

BaseGlobalPtr::~BaseGlobalPtr()
{
	for (BaseWeakPtr* const weak : weaks)
	{
		weak->CleanOwner();
	}
}


//BaseWeakPtr::~BaseWeakPtr()

void BaseWeakPtr::Set(const BaseGlobalPtr* inGlobal)
{
	if (globalPtr == inGlobal)
		return;

	if (globalPtr)
		Reset();

	globalPtr = inGlobal;

	// Bind into new owner
	if (globalPtr) {
		globalPtr->weaks.push_back(this);
		id = globalPtr->weaks.size() - 1;
	}
}

void BaseWeakPtr::MoveFrom(BaseWeakPtr&& other)
{
	id = eastl::move(other.id);
	globalPtr = other.globalPtr;
	other.globalPtr = nullptr;
	if (globalPtr)
	{
		globalPtr->weaks[id] = this;
		//owner->weaks.erase(&other);
		//owner->weaks.insert(this);
	}
}
