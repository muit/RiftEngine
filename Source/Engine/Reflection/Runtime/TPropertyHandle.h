// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <vector>
#include <functional>

#include "EngineTypes.h"
#include "PropertyHandle.h"
#include "Reflection/TProperty.h"

#if WITH_EDITOR
#include "Editor/Widgets/PropertyWidget.h"
#endif


/**
 * Base logic implementation for templated property handles.
 * Use TPropertyHandle instead
 */
template<typename VarType>
struct TBasePropertyHandle : public PropertyHandle
{
	friend TProperty<VarType>;

private:

	const Ptr<BaseObject> instance;
	const TProperty<VarType>* const prop;

public:
	TBasePropertyHandle() : instance{}, prop(nullptr) {}
	TBasePropertyHandle(const Ptr<BaseObject>& instance, const TProperty<VarType>* prop) : instance(instance), prop(prop) {}

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

	virtual bool IsValid() const override { return instance && prop != nullptr; }
	operator bool() const { return IsValid(); }


#if WITH_EDITOR
	virtual GlobalPtr<PropertyWidget> CreateWidget() { return {}; }
#endif
};


/**
* Points towards an existing reflected property in runtime
* Default type version
*/
template<typename VarType>
struct TPropertyHandle : public TBasePropertyHandle<VarType>
{
	TPropertyHandle(const Ptr<BaseObject>& instance, const TProperty<VarType>* prop) : TBasePropertyHandle(instance, prop) {}

#if WITH_EDITOR
	virtual GlobalPtr<PropertyWidget> CreateWidget() override { return {}; }
#endif
};


/**
* Points towards an existing reflected property in runtime
* uint8 type version
*/
template<>
struct TPropertyHandle<uint8> : public TBasePropertyHandle<uint8>
{
	TPropertyHandle(const Ptr<BaseObject>& instance, const TProperty<uint8>* prop) : TBasePropertyHandle(instance, prop) {}

#if WITH_EDITOR
	virtual GlobalPtr<PropertyWidget> CreateWidget() override { return {}; }
#endif
};


/**
* Points towards an existing reflected property in runtime
* uint8 type version
*/
template<>
struct TPropertyHandle<int32> : public TBasePropertyHandle<int32>
{
	TPropertyHandle(const Ptr<BaseObject>& instance, const TProperty<int32>* prop) : TBasePropertyHandle(instance, prop) {}

#if WITH_EDITOR
	virtual GlobalPtr<PropertyWidget> CreateWidget() override { return {}; }
#endif
};


/**
* Points towards an existing reflected property in runtime
* Name type version
*/
template<>
struct TPropertyHandle<Name> : public TBasePropertyHandle<Name>
{
	TPropertyHandle(const Ptr<BaseObject>& instance, const TProperty<Name>* prop) : TBasePropertyHandle(instance, prop) {}

#if WITH_EDITOR
	virtual GlobalPtr<PropertyWidget> CreateWidget() override { return {}; }
#endif
};
