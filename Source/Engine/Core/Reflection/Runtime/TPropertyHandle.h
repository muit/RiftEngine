// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <vector>
#include <functional>

#include "EngineTypes.h"
#include "PropertyHandle.h"
#include "Core/Reflection/Property.h"

#if WITH_EDITOR
#include "UI/Editor/Widgets/PropertyWidget.h"
#endif


/**
 * Base logic implementation for templated property handles.
 * Use TPropertyHandle instead
 */
template<typename VarType>
struct TPropertyHandle : public PropertyHandle
{
	friend TProperty<VarType>;

private:

	const Ptr<BaseObject> instance;
	const TProperty<VarType>* const prop;

public:
	TPropertyHandle() : instance{}, prop(nullptr) {}
	TPropertyHandle(const Ptr<BaseObject>& instance, const TProperty<VarType>* prop) : instance(instance), prop(prop) {}


	String GetName() const
	{
		if (prop)
			return prop->GetName();
		return "Invalid";
	}

	bool GetValue(VarType& value) const
	{
		if (IsValid())
		{
			VarType* ptr = prop->GetValuePtr(instance);
			if (ptr)
			{
				value = *ptr;
				return true;
			}
		}
		return false;
	}

	VarType* GetValuePtr() const
	{
		if (IsValid())
		{
			return prop->GetValuePtr(instance);
		}
		return nullptr;
	}

	bool SetValue(const VarType& value) const
	{
		if (IsValid())
		{
			VarType* ptr = prop->GetValuePtr(instance);
			if (ptr)
			{
				*ptr = value;
				return true;
			}
		}
		return false;
	}

	bool HasTag(ReflectionTags tag) const {
		return prop? prop->HasTag(tag) : false;
	}

	virtual bool IsValid() const override { return instance && prop != nullptr; }
	operator bool() const { return IsValid(); }
};
