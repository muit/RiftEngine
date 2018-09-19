// Copyright 2015-2019 Piperift - All rights reserved

#include "UIManager.h"


void UIManager::Prepare()
{
	// Setup style
	ImGui::StyleColorsDark();

#if WITH_EDITOR
	editor = Create<EditorManager>(ThisPtr());
#endif
}

void UIManager::Tick(float deltaTime)
{
	ImGui::NewFrame();

#if WITH_EDITOR
	editor->Tick(deltaTime);
#endif
}
