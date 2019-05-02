// Copyright 2015-2019 Piperift - All rights reserved

#include "BaseType.h"
#include "Property.h"


const Property* BaseType::FindProperty(const Name& propertyName) const
{
	const auto propIt = properties.find(propertyName);
	if (propIt == properties.end())
		return nullptr;
	else
		return dynamic_cast<const Property*>((*propIt).second.get());
}

const BaseType::PropertyMap& BaseType::GetAllProperties() const
{
	return properties;
}
