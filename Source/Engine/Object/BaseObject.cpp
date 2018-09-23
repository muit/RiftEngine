// Copyright 2015-2019 Piperift - All rights reserved

#include "BaseObject.h"
#include "Reflection/TClass.h"
#include "Object.h"

Class* BaseObject::GetClass()
{
	Object* thisObj = dynamic_cast<Object*>(this);
	if (thisObj)
		return thisObj->GetClass();
	return nullptr;
}
