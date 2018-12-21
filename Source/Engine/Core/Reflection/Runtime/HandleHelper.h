// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Core/Object/BaseObject.h"
#include "Core/Object/ObjectPtr.h"

#include "ClassHandle.h"
#include "TPropertyHandle.h"


class HandleHelper {
public:

	static eastl::shared_ptr<ClassHandle> CreateClassHandle(const Ptr<BaseObject>& instance)
	{
		return eastl::make_shared<ClassHandle>(instance);
	}

	template<typename VarType>
	static eastl::shared_ptr<PropertyHandle> CreatePropertyHandle(const Ptr<BaseObject>& instance, const TProperty<VarType>* property)
	{
		if (property && instance->GetClass() == property->GetType())
		{
			return eastl::shared_ptr<PropertyHandle>(new TPropertyHandle<VarType>(instance, property));
		}
		return {};
	}

	static eastl::shared_ptr<PropertyHandle> CreatePropertyHandle(const Ptr<BaseObject>& instance, const Property* property);
};
