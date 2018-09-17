#pragma once

#include <unordered_map>

#include "Property.h"


// Class will be specialized for each type at compile time and store
// the metadata for that type.
template <typename T>
struct Class {
	typedef std::unordered_map<std::string, std::unique_ptr<PropertyBase>> PropertyMap;

	static Class* GetStatic() { return &_class; }

	Class() {
		T::__meta_InitPropChain();
	}
	Class(const Class&) = delete;

	const PropertyMap& GetAllProperties() { return properties; }

	template<typename VarType>
	const Property<T, VarType>* FindProperty(const std::string& name) const
	{
		const auto propIt = properties.find(name);
		if (propIt == properties.end())
			return nullptr;
		else
			return dynamic_cast<const Property<T, VarType>*>((*propIt).second.get());
	}

	template<typename VarType>
	PropertyHandle<T, VarType> FindPropertyHandle(T& instance, const std::string& name) const
	{
		const Property<T, VarType>* prop{ FindProperty<VarType>(name) };
		if (prop)
		{
			return { instance, prop };
		}
		return {};
	}

public:
	/** GENERATION */
	template<typename VarType>
	void RegistryProperty(std::string&& name, std::function<VarType*(T&)>&& access, std::vector<std::string>&& tags)
	{
		//TODO: Support Tags
		properties.emplace(name, std::unique_ptr<PropertyBase>(new Property<T, VarType>(std::move(name), std::move(access))));
	}

private:

	PropertyMap properties;

	static Class _class;
};

template <typename T>
Class<T> Class<T>::_class {};
