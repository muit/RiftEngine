// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Property.h"
#include <functional>
#include <EASTL/shared_ptr.h>

#include "Core/Strings/Name.h"
#include "Class.h"
#include "ReflectionTags.h"
#include "Runtime/TPropertyHandle.h"


/**
 * Static information about a property
 */
template <typename VarType>
class TProperty : public Property {

	using Access = std::function<VarType*(BaseObject*)>;

private:

	Access access;


public:

	TProperty(BaseType* typePtr, const Name& typeName, Name&& name, Access&& access, ReflectionTags tags)
		: Property(typePtr, typeName, eastl::move(name), tags), access(access)
	{}

	virtual eastl::shared_ptr<PropertyHandle> CreateHandle(const Ptr<BaseObject>& instance) override {
		if (instance->GetClass() == GetType())
		{
			return eastl::shared_ptr<PropertyHandle>(new TPropertyHandle<VarType>(instance, this, access));
		}
		return {};
	}
};
