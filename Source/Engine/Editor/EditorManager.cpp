// Copyright 2015-2019 Piperift - All rights reserved

#include "EditorManager.h"
#include <imgui/imgui_stl.h>

#include "World.h"
#include "Core/Object/Object.h"
#include "Core/Assets/AssetPtr.h"
#include "Assets/Scene.h"


#if WITH_EDITOR

void EditorManager::Tick(float deltaTime)
{
	ZoneScopedNC("Editor", 0x459bd1);
	TickMainNavBar();
	//TickDocking();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);

	editors.RemoveIf([](const auto & editor) { return !editor.IsValid(); });
	for (const auto& editor : editors)
	{
		editor->Tick(deltaTime);
	}

	memory->OnTick(deltaTime);
	assetBrowser->OnTick(deltaTime);
}

void EditorManager::TickDocking()
{
	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	/*if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;*/

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	bool b_dockspace_open = true;
	ImGui::Begin("EditorLayoutWindow", &b_dockspace_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Dock space
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("EditorLayout");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}
	ImGui::End();
}

void EditorManager::TickMainNavBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Scene")) {}
			if (ImGui::MenuItem("Save Scene", "CTRL+S")) {
				if (TAssetPtr<Scene> scene = GetWorld()->GetActiveScene())
				{
					scene->Save();
				}
			}
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
			if (ImGui::MenuItem("Memory", (const char*)0, memory->IsOpenedPtr())) {}
			if (ImGui::MenuItem("Asset Browser", (const char*)0, assetBrowser->IsOpenedPtr())) {}

			for (const auto& editor : editors)
			{
				editor->ExpandViewsMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

#endif
