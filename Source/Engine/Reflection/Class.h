// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <unordered_map>

#include "EngineTypes.h"
#include "Property.h"


class Class {
protected:

	Name name;
	Class* parent;
	std::vector<Class*> children;

	typedef std::unordered_map<Name, std::unique_ptr<PropertyBase>> PropertyMap;

	PropertyMap properties;

public:

	Class() {}
	Class(const Class&) = delete;
	Class& operator=(const Class&) = delete;
	virtual ~Class() {}

	Name GetName() const { return name; }
	Class* GetParentClass() const { return parent; }

	template<bool bIncludeSelf = false, bool bIsFirstCall = true>
	void GetAllChildClasses(std::vector<Class*>& outChildren) {
		if(bIsFirstCall)
			outChildren.clear();

		if (bIncludeSelf)
			outChildren.push_back(this);

		for (auto* child : children)
		{
			outChildren.push_back(child);
			child->GetAllChildClasses<false, false>(outChildren);
		}
	}

	template<typename VarType>
	const Property<VarType>* FindProperty(const Name& name) const
	{
		const auto propIt = properties.find(name);
		if (propIt == properties.end())
			return nullptr;
		else
			return dynamic_cast<const Property<VarType>*>((*propIt).second.get());
	}

	const PropertyMap& GetAllProperties() { return properties; }

public:

	/** GENERATION */
	void RegistryChildren(Class* child)
	{
		children.push_back(child);
	}
};

// Class will be specialized for each type at compile time and store
// the metadata for that type.
template <typename T>
class TClass : public Class {
private:

	static TClass _class;


public:

	TClass() : Class() {
		// Registry inside parent
		T::__meta_RegistryClass();

		T::__meta_InitPropChain();
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
