// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <string>
#include <vector>
#include <functional>

#include "EngineTypes.h"
#include "Serialization/Archive.h"


/**
* Static information about a property
*/
class PropertyBase {
private:

	Name name;
	std::vector<Name> tags;


	PropertyBase() = delete;
	PropertyBase(PropertyBase&&) = delete;
	PropertyBase(const PropertyBase&) = delete;

protected:

	PropertyBase(Name&& name, std::vector<Name>&& tags)
		: name(name), tags(tags)
	{}

public:

	virtual ~PropertyBase() = default;

//	virtual void Serialize(void* instance, Archive& archive) const {}

	String GetName() { return name.ToString(); }
	bool HasTag(Name tag) const { return std::find(tags.begin(), tags.end(), std::move(tag)) != tags.end(); }
};


/**
 * Static information about a property
 */
template <typename VarType>
class Property : public PropertyBase {

private:

	std::function<VarType*(void*)> access;


public:

	Property(Name&& name, std::function<VarType*(void*)>&& access, std::vector<Name>&& tags)
		: PropertyBase(std::move(name), std::move(tags)), access(access)
	{}

	// TODO: Ensure class is correct or create object handle
	template<typename ClassType>
	void GetValue(ClassType& instance, VarType& value) const
	{
		value = *access(&instance);
	}

	template<typename ClassType>
	void SetValue(ClassType& instance, const VarType& value) const
	{
		*access(&instance) = value;
	}
};
