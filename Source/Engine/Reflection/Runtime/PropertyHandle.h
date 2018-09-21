// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <string>
#include <vector>
#include <functional>

#include "EngineTypes.h"
#include "Object/BaseObject.h"
#include "Object/ObjectPtr.h"
#include "Reflection/Property.h"


/**
 * Points towards an existing reflected property in runtime
 */
template<typename ObjectType, typename VarType>
struct PropertyHandle
{
	static_assert(std::is_convertible< ObjectType, BaseObject >::value, "Type is not an Object!");
	friend Property<VarType>;

private:

	const Ptr<ObjectType> instance;
	const Property<VarType>* const prop;

public:
	PropertyHandle() : instance{}, prop(nullptr) {}
	PropertyHandle(const Ptr<ObjectType>& instance, const Property<VarType>* prop) : instance(instance), prop(prop) {}

	bool GetValue(VarType& value) const
	{
		if (IsValid())
		{
			prop->GetValue(*instance, value);
			return true;
		}
		return false;
	}

	bool SetValue(const VarType& value) const
	{
		if (IsValid())
		{
			prop->SetValue(*instance, value);
			return true;
		}
		return false;
	}

	bool HasTag(Name tag) const {
		return prop? prop->HasTag(std::move(tag)) : false;
	}

	bool IsValid() const { return instance != nullptr && prop != nullptr; }
	operator bool() const { return IsValid(); }
};
