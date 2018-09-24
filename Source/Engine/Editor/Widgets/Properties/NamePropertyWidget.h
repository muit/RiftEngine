// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Editor/Widgets/PropertyWidget.h"
#include <imgui/imgui.h>
#include "Reflection/Runtime/TPropertyHandle.h"


class NamePropertyWidget : public PropertyWidget {
	CLASS(NamePropertyWidget, PropertyWidget)

	std::shared_ptr<TPropertyHandle<Name>> prop;

public:

	void Configure(const std::shared_ptr<TPropertyHandle<Name>>&  inProperty)
	{
		prop = inProperty;
	}

protected:

	virtual void Tick() override {
	}
};

#endif
