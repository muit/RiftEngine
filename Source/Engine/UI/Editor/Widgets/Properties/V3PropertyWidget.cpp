// Copyright 2015-2019 Piperift - All rights reserved

#include "V3PropertyWidget.h"
#include "Core/Math.h"
#include "Core/Reflection/ReflectionTags.h"

#if WITH_EDITOR


void V3PropertyWidget::Tick()
{
	ImGuiInputTextFlags flags = 0;
	if (!prop->HasTag(DetailsEdit))
		flags |= ImGuiInputTextFlags_ReadOnly;

	ImGui::PushID(idName.c_str());
	v3* const val = prop->GetValuePtr();
	ImGui::InputFloat3(displayName.c_str(), val->data(), "%g", flags);
	ImGui::PopID();
}

#endif
