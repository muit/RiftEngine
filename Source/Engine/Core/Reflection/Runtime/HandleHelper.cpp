// Copyright 2015-2019 Piperift - All rights reserved

#include "HandleHelper.h"


eastl::shared_ptr<PropertyHandle> HandleHelper::CreatePropertyHandle(const Ptr<BaseObject>& instance, const Property* property)
{
	if (property && instance->GetClass() == property->GetType())
	{
		if (const auto* propUInt8 = dynamic_cast<const TProperty<uint8>*>(property)) {
			return CreatePropertyHandle(instance, propUInt8);
		}
		else if (const auto* propInt32 = dynamic_cast<const TProperty<int32>*>(property)) {
			return CreatePropertyHandle(instance, propInt32);
		}
		else if (const auto* propName = dynamic_cast<const TProperty<Name>*>(property)) {
			return CreatePropertyHandle(instance, propName);
		}
		else if (const auto* propString = dynamic_cast<const TProperty<String>*>(property)) {
			return CreatePropertyHandle(instance, propString);
		}
	}
	return {};
}
