// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Class.h"
#include "TProperty.h"


// Class will be specialized for each type at compile time and store
// the metadata for that type.
template <typename ObjectType>
class TClass : public Class {
	static_assert(eastl::is_convertible< ObjectType, BaseObject >::value, "Type is not an Object!");
private:

	static TClass _class;


public:

	TClass() : Class() {
		ObjectType::__meta_RegistryClass();
		ObjectType::__meta_RegistryProperties();
	}

public:

	virtual GlobalPtr<BaseObject> CreateInstance(const Ptr<BaseObject>& owner) override {
		GlobalPtr<BaseObject> ptr = GlobalPtr<ObjectType>::Create(owner);
		return ptr;
	}


	/** GENERATION */

	/** Registry a class
	*/
	void Registry(Name&& inName)
	{
		name = inName;
	}

	/** Registry a class with a parent */
	template<typename Super>
	void Registry(Name&& inName)
	{
		parent = Super::StaticClass();
		parent->RegistryChildren(this);
		Registry(eastl::move(inName));
	}

	/** Registry a class's tags */
	void RegistryTags(ReflectionTags inTags)
	{
		tags = inTags;
	}

	template<typename VarType>
	void RegistryProperty(Name&& name, eastl::function<VarType*(BaseObject*)>&& access, ReflectionTags tags)
	{
		properties.emplace(name, eastl::unique_ptr<Property>(
			new TProperty<VarType>(
				this,
				GetReflectableName<VarType>(),
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
