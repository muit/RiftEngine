#pragma once

#include <string>
#include <vector>
#include <functional>

#include "EngineTypes.h"

/**
* Static information about a property
*/
class PropertyBase {
private:

	String name;
	std::vector<Name> tags;


	PropertyBase() = delete;
	PropertyBase(PropertyBase&&) = delete;
	PropertyBase(const PropertyBase&) = delete;

protected:

	PropertyBase(String&& name, std::vector<Name>&& tags)
		: name(name), tags(tags)
	{}

public:

	bool HasTag(Name tag) const {
		return std::find(tags.begin(), tags.end(), std::move(tag)) != tags.end();
	}

	virtual ~PropertyBase() = default;

	String GetName() { return name; }
};

/**
 * Static information about a property
 */
template <typename ClassType, typename VarType>
class Property : public PropertyBase {

private:

	std::function<VarType*(ClassType&)> access;

public:

	Property(String&& name, std::function<VarType*(ClassType&)>&& access, std::vector<Name>&& tags)
		: PropertyBase(std::move(name), std::move(tags)), access(access)
	{}

	void GetValue(ClassType& instance, VarType& value) const
	{
		value = *access(instance);
	}

	void SetValue(ClassType& instance, const VarType& value) const
	{
		*access(instance) = value;
	}
};

/**
 * Points towards an existing reflected property in runtime
 */
template<typename ClassType, typename VarType>
struct PropertyHandle
{
	friend Property<ClassType, VarType>;

private:

	ClassType* const instance;
	const Property<ClassType, VarType>* const prop;

public:
	PropertyHandle() : instance(nullptr), prop(nullptr) {}
	PropertyHandle(ClassType& instance, const Property<ClassType, VarType>* prop) : instance(&instance), prop(prop) {}

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
