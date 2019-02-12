// Copyright 2015-2019 Piperift - All rights reserved

#include "BoolPropertyWidget.h"


#if WITH_EDITOR

void BoolPropertyWidget::Tick()
{
	ImGui::PushID(idName.c_str());

	ImGui::Checkbox(displayName.c_str(), GetHandle()->GetValuePtr());

	ImGui::PopID();
}

#endif
