// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <unordered_map>

#include "EngineTypes.h"
#include "Property.h"


// Class will be specialized for each type at compile time and store
// the metadata for that type.
template <typename T>
struct Class {
	typedef std::unordered_map<Name, std::unique_ptr<PropertyBase>> PropertyMap;

	static Class* GetStatic() { return &_class; }

	Class() {
		T::__meta_InitPropChain();
	}
	Class(const Class&) = delete;

	const PropertyMap& GetAllProperties() { return properties; }

	template<typename VarType>
	const Property<VarType>* FindProperty(const Name& name) const
	{
		const auto propIt = properties.find(name);
		if (propIt == properties.end())
			return nullptr;
		else
			return dynamic_cast<const Property<T, VarType>*>((*propIt).second.get());
	}

	template<typename VarType>
	PropertyHandle<T, VarType> FindPropertyHandle(T& instance, const String& name) const
	{
		const Property<VarType>* prop{ FindProperty<VarType>(name) };
		if (prop)
		{
			return { instance, prop };
		}
		return {};
	}

	void Serialize(T& instance, Archive& archive)
	{
		//TODO: Iterate properties and serialize them
		/* for(const PropertyBase& property : properties)
		{
			property.Serialize(instance, archive);
		} */
	}

public:
	/** GENERATION */
	template<typename VarType>
	void RegistryProperty(Name&& name, std::function<VarType*(void*)>&& access, std::vector<Name>&& tags)
	{
		properties.emplace(name, std::unique_ptr<PropertyBase>(
			new Property<VarType>(
				std::move(name),
				std::move(access),
				std::move(tags)
			)
		));
	}

private:

	PropertyMap properties;

	static Class _class;
};

template <typename T>
Class<T> Class<T>::_class {};
