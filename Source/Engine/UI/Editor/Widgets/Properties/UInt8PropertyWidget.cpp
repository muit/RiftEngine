// Copyright 2015-2019 Piperift - All rights reserved

#include "UInt8PropertyWidget.h"
#include "Core/Math/Math.h"

#if WITH_EDITOR

void UInt8PropertyWidget::Tick()
{
	ImGui::PushID(idName.c_str());

	int32 value = *prop->GetValuePtr();
	ImGui::InputInt(displayName.c_str(), &value);

	value = Math::Clamp(value, 0, 255);
	if (value != *prop->GetValuePtr())
	{
		prop->SetValue((uint8)value);
	}

	ImGui::PopID();
}

#endif
