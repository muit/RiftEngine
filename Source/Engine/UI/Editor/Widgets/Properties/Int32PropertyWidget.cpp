// Copyright 2015-2019 Piperift - All rights reserved

#include "Int32PropertyWidget.h"


#if WITH_EDITOR

void Int32PropertyWidget::Tick()
{
	ImGui::PushID(idName.c_str());

	ImGui::InputInt(displayName.c_str(), prop->GetValuePtr());

	ImGui::PopID();
}

#endif
