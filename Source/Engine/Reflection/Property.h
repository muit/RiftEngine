#pragma once

#include <string>
#include <vector>
#include <functional>

/**
* Static information about a property
*/
class PropertyBase {
private:

	std::string name;
	std::vector<std::string> tags;


	PropertyBase() = delete;
	PropertyBase(PropertyBase&&) = delete;
	PropertyBase(const PropertyBase&) = delete;

protected:

	PropertyBase(std::string&& name) : name(name) {}

public:

	virtual ~PropertyBase() = default;

	std::string GetName() { return name; }
};

/**
 * Static information about a property
 */
template <typename ClassType, typename VarType>
class Property : public PropertyBase {

private:

	std::function<VarType*(ClassType&)> access;

public:

	Property(std::string&& name, std::function<VarType*(ClassType&)>&& access)
		: PropertyBase(std::move(name)), access(access)
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

	bool IsValid() const { return instance != nullptr && prop != nullptr; }
	operator bool() const { return IsValid(); }
};
