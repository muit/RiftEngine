// Copyright 2015-2019 Piperift - All rights reserved

#include "EditorManager.h"
#include "imgui/imgui_stl.h"


#if WITH_EDITOR

void EditorManager::Tick(float deltaTime)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Scene")) {}
			if (ImGui::MenuItem("Save Scene", "CTRL+S", false, false)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Details", (const char*)0, details->IsOpenedPtr())) {}
			if (ImGui::MenuItem("Memory", (const char*)0, memory->IsOpenedPtr())) {}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	details->OnTick();
	memory->OnTick();


	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);
}

#endif
