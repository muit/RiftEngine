// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/shared_ptr.h>

#include "Core/Object/BaseObject.h"
#include "Core/Strings/Name.h"
#include "Core/Strings/String.h"
#include "ReflectionTags.h"


class BaseType;
struct PropertyHandle;

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

	const String& GetName() const { return name.ToString(); }

	bool HasTag(ReflectionTags tag)      const { return HasAnyTags(tag); }
	bool HasAllTags(ReflectionTags inTags) const { return (tags & inTags) == inTags; }
	bool HasAnyTags(ReflectionTags inTags) const { return (tags & inTags) > 0; }

	BaseType* GetType() const { return typePtr; }
	Name GetTypeName() const { return typeName; }

	virtual eastl::shared_ptr<PropertyHandle> CreateHandle(const Ptr<BaseObject>& instance) = 0;
};
