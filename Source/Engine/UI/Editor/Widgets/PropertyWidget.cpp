// Copyright 2015-2019 Piperift - All rights reserved

#include "PropertyWidget.h"

#if WITH_EDITOR

#include "Core/Reflection/Runtime/TPropertyHandle.h"
#include "Properties/UInt8PropertyWidget.h"
#include "Properties/Int32PropertyWidget.h"
#include "Properties/FloatPropertyWidget.h"
#include "Properties/NamePropertyWidget.h"
#include "Properties/StringPropertyWidget.h"
#include "Properties/V3PropertyWidget.h"
#include "Properties/V2PropertyWidget.h"


GlobalPtr<PropertyWidget> PropertyWidget::NewPropertyWidget(const eastl::shared_ptr<PropertyHandle>& prop)
{
	if (prop)
	{
		// Ordered by estimated usage
		// #TODO: Switch to native pointers
		if (auto propFloat = eastl::dynamic_pointer_cast<TPropertyHandle<float>>(prop)) {
			return New<FloatPropertyWidget>(propFloat);
		}
		else if (auto propInt32 = eastl::dynamic_pointer_cast<TPropertyHandle<i32>>(prop)) {
			return New<Int32PropertyWidget>(propInt32);
		}
		else if (auto propUInt8 = eastl::dynamic_pointer_cast<TPropertyHandle<u8>>(prop)) {
			return New<UInt8PropertyWidget>(propUInt8);
		}
		else if (auto propName = eastl::dynamic_pointer_cast<TPropertyHandle<Name>>(prop)) {
			return New<NamePropertyWidget>(propName);
		}
		else if (auto propString = eastl::dynamic_pointer_cast<TPropertyHandle<String>>(prop)) {
			return New<StringPropertyWidget>(propString);
		}
		else if (auto propV3 = eastl::dynamic_pointer_cast<TPropertyHandle<v3>>(prop)) {
			return New<V3PropertyWidget>(propV3);
		}
		else if (auto propV2 = eastl::dynamic_pointer_cast<TPropertyHandle<v2>>(prop)) {
			return New<V2PropertyWidget>(propV2);
		}
	}
	return {};
}

#endif
