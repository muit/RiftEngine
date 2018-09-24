// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Editor/Widgets/PropertyWidget.h"
#include "Reflection/Runtime/TPropertyHandle.h"


class NamePropertyWidget : public PropertyWidget {
	CLASS(NamePropertyWidget, PropertyWidget)

	std::shared_ptr<TPropertyHandle<Name>> prop;

	String currentValue;
	String lastValue;

public:

	NamePropertyWidget() : Super(), lastValue{}, currentValue{} {}

	void Configure(const std::shared_ptr<TPropertyHandle<Name>>&  inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		StringUtils::ToSentenceCase(idName, displayName);

		// Avoid event being called on first tick:
		lastValue = prop->GetValuePtr()->ToString();
	}

	void OnValueChanged();

protected:

	virtual void Tick() override;
};

#endif
