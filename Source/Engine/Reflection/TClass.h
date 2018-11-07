// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Class.h"
#include "Object/BaseObject.h"
#include "Property.h"


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

	/** Registry a class's tags */
	void RegistryClassTags(ReflectionTags inTags)
	{
		tags = inTags;
	}

	template<typename VarType>
	void RegistryProperty(Name&& name, eastl::function<VarType*(BaseObject*)>&& access, ReflectionTags tags)
	{
		properties.emplace(name, eastl::unique_ptr<Property>(
			new TProperty<VarType>(
				this,
				ReflectionTypeTraits<VarType>::name,
				eastl::move(name),
				eastl::move(access),
				tags
			)
		));
	}

	static TClass* GetStatic() { return &_class; }
};

template <typename T>
TClass<T> TClass<T>::_class {};
