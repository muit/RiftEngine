// Copyright 2015-2019 Piperift - All rights reserved

#include "FloatPropertyWidget.h"
#include "Util/Math.h"

#if WITH_EDITOR

void FloatPropertyWidget::Tick()
{
	ImGui::PushID(idName.c_str());

	ImGui::InputFloat(displayName.c_str(), prop->GetValuePtr());

	ImGui::PopID();
}

#endif
