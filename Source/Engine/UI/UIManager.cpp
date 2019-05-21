// Copyright 2015-2019 Piperift - All rights reserved

#include "UIManager.h"
#include <imgui/imgui_impl_sdl.h>

#include "Core/Log.h"
#include "Tools/Profiler.h"


void UIManager::Prepare()
{
	Log::Message("UI Start-Up");

	// Setup style
	ImGui::StyleColorsDark();

#if WITH_EDITOR
	editor = Create<EditorManager>(Self());
#endif
}

void UIManager::OnSDLEvent(SDL_Event* ev)
{
	ImGui_ImplSDL2_ProcessEvent(ev);
}

void UIManager::Tick(float deltaTime)
{
	ScopedGameZone("UI Logic");

#if WITH_EDITOR
	editor->Tick(deltaTime);
#endif

	// Prepares the Draw List
	ImGui::Render();
}
