// Copyright 2015-2019 Piperift - All rights reserved

#include "PropertyWidget.h"

#if WITH_EDITOR
#include "Reflection/Runtime/TPropertyHandle.h"
#include "Properties/UInt8PropertyWidget.h"
#include "Properties/Int32PropertyWidget.h"
#include "Properties/NamePropertyWidget.h"
#include "Properties/StringPropertyWidget.h"


GlobalPtr<PropertyWidget> PropertyWidget::NewPropertyWidget(PropertyHandle* prop)
{
	if (prop)
	{
		if (auto* propUInt8 = dynamic_cast<TPropertyHandle<uint8>*>(prop)) {
			return New<UInt8PropertyWidget>(propUInt8);
		}
		else if (auto* propInt32 = dynamic_cast<TPropertyHandle<int32>*>(prop)) {
			return New<Int32PropertyWidget>(propInt32);
		}
		else if (auto* propName = dynamic_cast<TPropertyHandle<Name>*>(prop)) {
			return New<NamePropertyWidget>(propName);
		}
		else if (auto* propString = dynamic_cast<TPropertyHandle<String>*>(prop)) {
			return New<StringPropertyWidget>(propString);
		}
	}
	return {};
}

#endif
