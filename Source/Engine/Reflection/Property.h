// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <string>
#include <vector>
#include <functional>


#include "EngineTypes.h"
#include "Object/BaseObject.h"
#include "Object/ObjectPtr.h"
#include "Serialization/Archive.h"


/**
* Static information about a property
*/
class Property {
private:

	Class* classPtr;
	Name typeName;
	Name name;
	std::vector<Name> tags;


	Property() = delete;
	Property(Property&&) = delete;
	Property(const Property&) = delete;

protected:

	Property(Class* classPtr, const Name& typeName, Name&& name, std::vector<Name>&& tags)
		: typeName(typeName), classPtr(classPtr), name(name), tags(tags)
	{}

public:

	virtual ~Property() = default;

	//	virtual void Serialize(void* instance, Archive& archive) const {}

	String GetName() { return name.ToString(); }
	bool HasTag(Name tag) const { return std::find(tags.begin(), tags.end(), std::move(tag)) != tags.end(); }

	Class* GetClass() const { return classPtr; }
	Name GetTypeName() const { return typeName; }
};


/**
 * Static information about a property
 */
template <typename VarType>
class TProperty : public Property {

private:

	std::function<VarType*(BaseObject*)> access;


public:

	TProperty(Class* classPtr, const Name& typeName, Name&& name, std::function<VarType*(BaseObject*)>&& access, std::vector<Name>&& tags)
		: Property(classPtr, typeName, std::move(name), std::move(tags)), access(access)
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
