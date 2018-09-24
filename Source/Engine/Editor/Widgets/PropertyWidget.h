// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include <imgui/imgui.h>
#include "UI/Widget.h"
#include "Reflection/Runtime/PropertyHandle.h"


class PropertyWidget : public Widget {
	CLASS(PropertyWidget, Widget)

protected:

	virtual void Tick() override {}

public:
	
	static GlobalPtr<PropertyWidget> NewPropertyWidget(const std::shared_ptr<PropertyHandle>& prop);
};

#endif
