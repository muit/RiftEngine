// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "../PropertyWidget.h"

#include "Core/Reflection/Runtime/TPropertyHandle.h"


class NamePropertyWidget : public PropertyWidget {
	CLASS(NamePropertyWidget, PropertyWidget)

	eastl::shared_ptr<TPropertyHandle<Name>> prop;

	String currentValue;
	String lastValue;

public:

	NamePropertyWidget() : Super(), lastValue{}, currentValue{} {}

	void Configure(const eastl::shared_ptr<TPropertyHandle<Name>>&  inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		StringUtils::ToSentenceCase(idName, displayName);
	}

	void OnValueChanged();

protected:

	virtual void Tick() override;
};

#endif
