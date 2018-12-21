// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <string>
#include <vector>
#include <functional>

#include "EngineTypes.h"
#include "Object/BaseObject.h"
#include "Object/ObjectPtr.h"
#include "Serialization/Archive.h"
#include "ReflectionTags.h"

class BaseType;


/**
* Static information about a property
*/
class Property {
private:

	BaseType* typePtr;
	Name typeName;
	Name name;
	ReflectionTags tags;


	Property() = delete;
	Property(Property&&) = delete;
	Property(const Property&) = delete;

protected:

	Property(BaseType* typePtr, const Name& typeName, Name&& name, ReflectionTags tags)
		: typeName(typeName), typePtr(typePtr), name(name), tags(tags)
	{}

public:

	virtual ~Property() = default;

	String GetName() const { return name.ToString(); }
	bool HasTag(ReflectionTags tag) const { return (tags | tag) > 0; }

	BaseType* GetType() const { return typePtr; }
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

	TProperty(BaseType* typePtr, const Name& typeName, Name&& name, std::function<VarType*(BaseObject*)>&& access, ReflectionTags tags)
		: Property(typePtr, typeName, std::move(name), tags), access(access)
	{}

	// Will be nullptr if class is not correct
	VarType* GetValuePtr(const Ptr<BaseObject>& instance) const
	{
		return access(*instance);
	}
};
