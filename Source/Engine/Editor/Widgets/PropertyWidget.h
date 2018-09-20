// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include <imgui/imgui.h>
#include "UI/Widget.h"

class PropertyWidget : public Widget {
	virtual void Tick() override {
		if (ImGui::Button("My Property")) {}
	}
};

#endif
