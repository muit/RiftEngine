// Copyright 2015-2019 Piperift - All rights reserved

#include "ObjectPtr.h"
#include "Object.h"


void BaseGlobalPtr::MoveFrom(BaseGlobalPtr&& other)
{
	ptr = std::move(other.ptr);
	weaks = std::move(other.weaks);

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


void BaseWeakPtr::Set(const BaseGlobalPtr* inOwner)
{
	if (owner && owner != inOwner)
		Reset();

	owner = inOwner;

	// Bind into new owner
	if (owner)
		owner->weaks.insert(this);
}

void BaseWeakPtr::MoveFrom(BaseWeakPtr&& other)
{
	owner = other.owner;
	other.owner = nullptr;
	if (owner)
	{
		owner->weaks.erase(&other);
		owner->weaks.insert(this);
	}
}
