// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"

#include <EASTL/unordered_map.h>
#include <EASTL/unique_ptr.h>

#include "EngineTypes.h"
#include "ReflectionTags.h"
#include "Property.h"


class BaseType {
protected:

	Name name;
	ReflectionTags tags;

	typedef eastl::unordered_map<Name, eastl::unique_ptr<Property>> PropertyMap;

	PropertyMap properties;

public:

	BaseType() {}
	BaseType(const BaseType&) = delete;
	BaseType& operator=(const BaseType&) = delete;
	virtual ~BaseType() {}

	const Name& GetName() const { return name; }
	const String& GetSName() const { return GetName().ToString(); }

	const Property* FindProperty(const Name& propertyName) const;

	const PropertyMap& GetAllProperties() const;
};
