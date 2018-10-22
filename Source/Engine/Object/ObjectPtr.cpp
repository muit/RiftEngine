// Copyright 2015-2019 Piperift - All rights reserved

#include "ObjectPtr.h"
#include "Object.h"


void BaseGlobalPtr::MoveFrom(BaseGlobalPtr&& other)
{
	ptr = std::move(other.ptr);
	weaks = std::move(other.weaks);

	// Update owner pointers to owner
	for (BaseWeakPtr* const weak : weaks)
	{
		weak->owner = this;
	}
}

BaseGlobalPtr::~BaseGlobalPtr()
{
	for (BaseWeakPtr* const weak : weaks)
	{
		weak->Reset();
	}
}
