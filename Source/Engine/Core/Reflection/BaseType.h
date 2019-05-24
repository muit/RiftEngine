// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"

#include <EASTL/unique_ptr.h>

#include "EngineTypes.h"
#include "Core/Containers/Map.h"
#include "ReflectionTags.h"
#include "Property.h"


class BaseType {
protected:

	Name name;
	ReflectionTags tags;

	using PropertyMap = TMap<Name, eastl::unique_ptr<Property>>;

	PropertyMap properties;

public:

	BaseType() : tags{} {
		properties.SetEmptyKey(Name::None());
	}
	BaseType(const BaseType&) = delete;
	BaseType& operator=(const BaseType&) = delete;
	virtual ~BaseType() {}

	const Name& GetName() const { return name; }
	const String& GetSName() const { return GetName().ToString(); }

	const Property* FindProperty(const Name& propertyName) const;

	const PropertyMap& GetAllProperties() const;
};
