// Copyright 2015-2019 Piperift - All rights reserved

#include "BaseObject.h"
#include "Reflection/TClass.h"
#include "Object.h"

Class* BaseObject::GetClass() const
{
	const Object* thisObj = dynamic_cast<const Object*>(this);
	if (thisObj)
		return thisObj->GetClass();
	return nullptr;
}

Ptr<Object> BaseObject::GetSelf() const
{
	const Object* thisObj = dynamic_cast<const Object*>(this);
	if (thisObj)
		return thisObj->GetSelf();
	return {};
}
