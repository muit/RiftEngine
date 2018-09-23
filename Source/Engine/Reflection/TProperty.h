// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <functional>

#include "EngineTypes.h"
#include "Object/BaseObject.h"
#include "Property.h"


/**
 * Static information about a property
 */
template <typename VarType>
class TProperty : public Property {

private:

	std::function<VarType*(BaseObject*)> access;


public:

	TProperty(Name&& name, std::function<VarType*(BaseObject*)>&& access, std::vector<Name>&& tags)
		: Property(std::move(name), std::move(tags)), access(access)
	{}

	// TODO: Ensure class is correct or create object handle
	void GetValue(const Ptr<BaseObject>& instance, VarType& value) const
	{
		value = *access(instance);
	}

	void SetValue(const Ptr<BaseObject>& instance, const VarType& value) const
	{
		*access(&instance) = value;
	}
};
