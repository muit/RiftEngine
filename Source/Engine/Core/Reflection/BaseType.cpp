// Copyright 2015-2019 Piperift - All rights reserved

#include "BaseType.h"
#include "Property.h"


const Property* BaseType::FindProperty(const Name& propertyName) const
{
	const auto* prop = properties.Find(propertyName);
	return prop? prop->get() : nullptr;
}

const BaseType::PropertyMap& BaseType::GetAllProperties() const
{
	return properties;
}
