// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Class.h"
#include "Object/BaseObject.h"
#include "Runtime/PropertyHandle.h"


// Class will be specialized for each type at compile time and store
// the metadata for that type.
template <typename ObjectType>
class TClass : public Class {
	static_assert(std::is_convertible< ObjectType, BaseObject >::value, "Type is not an Object!");
private:

	static TClass _class;


public:

	TClass() : Class() {
		ObjectType::__meta_RegistryClass();
		ObjectType::__meta_RegistryProperties();
	}

	template<typename VarType>
	PropertyHandle<ObjectType, VarType> FindPropertyHandle(const Ptr<ObjectType>& instance, const String& name) const
	{
		const Property<VarType>* prop{ FindProperty<VarType>(name) };
		if (prop)
		{
			return { instance, prop };
		}
		return {};
	}

	/*void Serialize(T& instance, Archive& archive)
	{
		//TODO: Iterate properties and serialize them
		for(const PropertyBase& property : properties)
		{
			property.Serialize(instance, archive);
		}
	}*/

public:

	/** GENERATION */

	/** Registry a class
	*/
	void RegistryClass(const Name& inName)
	{
		name = inName;
	}

	/** Registry a class with a parent */
	template<typename Super>
	void RegistryClass(const Name& inName)
	{
		parent = Super::StaticClass();
		parent->RegistryChildren(this);
		RegistryClass(inName);
	}

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

	static TClass* GetStatic() { return &_class; }
};

template <typename T>
TClass<T> TClass<T>::_class {};
