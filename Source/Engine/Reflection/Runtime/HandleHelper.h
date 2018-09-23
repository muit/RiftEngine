// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Object/BaseObject.h"
#include "Object/ObjectPtr.h"

#include "ClassHandle.h"
#include "TPropertyHandle.h"


class HandleHelper {

	static std::shared_ptr<ClassHandle> CreateClassHandle(const Ptr<BaseObject>& instance)
	{
		Class* instanceClass = instance->GetClass();
		return std::make_shared<ClassHandle>(instanceClass);
	}

	static std::shared_ptr<ClassHandle> CreateClassHandle(const Class* instanceClass)
	{
		return std::make_shared<ClassHandle>(instanceClass);
	}


	template<typename ObjectType, typename VarType>
	static std::shared_ptr<PropertyHandle> CreatePropertyHandle(const Ptr<ObjectType>& instance, const TProperty<ObjectType, VarType>& property)
	{
		return std::shared_ptr<PropertyHandle>(new TPropertyHandle<ObjectType, VarType>(castedInstance, property));
	}
};
