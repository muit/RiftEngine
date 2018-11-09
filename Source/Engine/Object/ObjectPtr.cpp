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
		weak->owner = this;
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

void BaseWeakPtr::Set(const BaseGlobalPtr* inOwner)
{
	if (owner == inOwner)
		return;

	if (owner)
		Reset();

	owner = inOwner;

	// Bind into new owner
	if (owner) {
		owner->weaks.push_back(this);
		id = owner->weaks.size() - 1;
		//owner->weaks.insert(this);
	}
}

void BaseWeakPtr::MoveFrom(BaseWeakPtr&& other)
{
	owner = other.owner;
	other.owner = nullptr;
	if (owner)
	{
		owner->weaks[id] = this;
		//owner->weaks.erase(&other);
		//owner->weaks.insert(this);
	}
}
