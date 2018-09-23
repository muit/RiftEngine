// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Editor/Widgets/PropertyWidget.h"
#include <imgui/imgui.h>
#include "Reflection/Runtime/TPropertyHandle.h"


class Int32PropertyWidget : public PropertyWidget {
	CLASS(Int32PropertyWidget, PropertyWidget)

	TPropertyHandle<int32>* prop;

public:

	void Configure(TPropertyHandle<int32>* inProperty)
	{
		prop = inProperty;
	}

protected:

	virtual void Tick() override {
		ImGui::InputInt(prop->GetName().c_str(), prop->GetValuePtr());
	}
};

#endif
