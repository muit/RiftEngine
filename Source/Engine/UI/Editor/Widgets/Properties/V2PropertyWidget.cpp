// Copyright 2015-2019 Piperift - All rights reserved

#include "V2PropertyWidget.h"
#include "Core/Math.h"
#include "Core/Reflection/ReflectionTags.h"

#if WITH_EDITOR

void V2PropertyWidget::Tick()
{
	ImGuiInputTextFlags flags = 0;
	if (!prop->HasTag(DetailsEdit))
		flags |= ImGuiInputTextFlags_ReadOnly;

	ImGui::PushID(idName.c_str());
	v2* const val = prop->GetValuePtr();
	ImGui::InputFloat2(displayName.c_str(), val->data(), "%g", flags);
	ImGui::PopID();
}

#endif
