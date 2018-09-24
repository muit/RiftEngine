// Copyright 2015-2019 Piperift - All rights reserved

#include "UInt8PropertyWidget.h"

#if WITH_EDITOR


#endif

void UInt8PropertyWidget::Tick()
{
	ImGui::PushID(idName.c_str());

	ImGui::Text(displayName.c_str());
	//ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 200, 10);
	//ImGui::InputInt("", prop->GetValuePtr());

	ImGui::PopID();
}
