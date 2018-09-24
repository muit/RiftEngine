// Copyright 2015-2019 Piperift - All rights reserved

#include "StringPropertyWidget.h"
#include "imgui/imgui_stl.h"

#if WITH_EDITOR

void StringPropertyWidget::Tick()
{
	ImGui::PushID(name.c_str());
	ImGui::Text(name.c_str());
	ImGui::SameLine();
	ImGui::InputText("", prop->GetValuePtr());
	ImGui::PopID();
}

void StringPropertyWidget::GetValue()
{
}

#endif
