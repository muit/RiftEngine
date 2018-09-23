// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Object/BaseObject.h"
#include "Object/ObjectPtr.h"

#include "ClassHandle.h"
#include "TPropertyHandle.h"


class HandleHelper {
public:

	static std::shared_ptr<ClassHandle> CreateClassHandle(const Ptr<BaseObject>& instance)
	{
		return std::make_shared<ClassHandle>(instance);
	}

	template<typename VarType>
	static std::shared_ptr<PropertyHandle> CreatePropertyHandle(const Ptr<BaseObject>& instance, const TProperty<VarType>* property)
	{
		if (property && instance->GetClass() == property->GetClass())
		{
			return std::shared_ptr<PropertyHandle>(new TPropertyHandle<VarType>(instance, property));
		}
		return {};
	}

	static std::shared_ptr<PropertyHandle> CreatePropertyHandle(const Ptr<BaseObject>& instance, const Property* property);
};
