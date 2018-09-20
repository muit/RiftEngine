// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include <imgui/imgui.h>
#include "UI/Widget.h"
#include "Reflection/Class.h"


class PropertyWidget : public Widget {

	String propertyName;

public:

	void Configure(const Class* inClass)
	{
		propertyName = inClass->GetName().ToString();
	}

protected:

	virtual void Tick() override {
		ImGui::Text(propertyName.c_str());
	}
};

#endif
