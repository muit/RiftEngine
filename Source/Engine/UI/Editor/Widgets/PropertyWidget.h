// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Widget.h"

#include <imgui/imgui.h>

#include "Strings/String.h"
#include "Reflection/Runtime/PropertyHandle.h"


class PropertyWidget : public Widget {
	CLASS(PropertyWidget, Widget)

protected:

	String idName;
	String displayName;


	virtual void Tick() override {}

public:

	static GlobalPtr<PropertyWidget> NewPropertyWidget(const eastl::shared_ptr<PropertyHandle>& prop);
};

#endif
