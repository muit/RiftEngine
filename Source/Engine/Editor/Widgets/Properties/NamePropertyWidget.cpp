// Copyright 2015-2019 Piperift - All rights reserved

#include "NamePropertyWidget.h"
#include "imgui/imgui_stl.h"

#if WITH_EDITOR

void NamePropertyWidget::OnValueChanged()
{
	const Name newValue{ currentValue };
	prop->SetValue(newValue);
}

void NamePropertyWidget::Tick()
{
	ImGui::PushID(idName.c_str());
	{
		lastValue = currentValue;
		currentValue = prop->GetValuePtr()->ToString();
		ImGui::InputText(displayName.c_str(), &currentValue);
	}
	ImGui::PopID();

	if (lastValue != currentValue)
	{
		OnValueChanged();
	}
}

#endif
