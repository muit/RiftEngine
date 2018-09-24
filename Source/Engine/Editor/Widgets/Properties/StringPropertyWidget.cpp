// Copyright 2015-2019 Piperift - All rights reserved

#include "StringPropertyWidget.h"
#include "imgui/imgui_stl.h"

#if WITH_EDITOR

void StringPropertyWidget::Tick()
{
	ImGui::PushID(idName.c_str());
	ImGui::InputText(displayName.c_str(), prop->GetValuePtr());
	ImGui::PopID();
}

void StringPropertyWidget::GetValue()
{
}

#endif
