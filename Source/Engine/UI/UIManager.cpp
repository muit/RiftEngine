// Copyright 2015-2019 Piperift - All rights reserved

#include "UIManager.h"
#include <imgui/imgui_impl_sdl.h>

#include "Core/Log.h"
#include "Tools/Profiler.h"


TMap<Name, ImFont*> UIManager::fonts {};

void UIManager::Prepare()
{
	Log::Message("UI Start-Up");

	// Setup style
	AddFont("Karla", FileSystem::GetAssetsPath() / "Fonts/karla_regular.ttf");
	AddFont("KarlaBold", FileSystem::GetAssetsPath() / "Fonts/karla_bold.ttf");
	AddFont("KarlaItalic", FileSystem::GetAssetsPath() / "Fonts/karla_italic.ttf");
	AddFont("KarlaBoldItalic", FileSystem::GetAssetsPath() / "Fonts/karla_bold_italic.ttf");
	ImGui::StyleColorsDark();
	ApplyStyle();

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

void UIManager::AddFont(Name name, Path path, u8 size)
{
	String sPath = FileSystem::ToString(path);

	if (!FileSystem::FileExists(path))
	{
		Log::Error("Font doesn't exist in folder '%s'", sPath.c_str());
		return;
	}

	if (name.IsNone())
	{
		Log::Error("No name provided for font '%s'", sPath.c_str());
		return;
	}

	auto& io = ImGui::GetIO();
	ImFont* loadedFont = io.Fonts->AddFontFromFileTTF(sPath.c_str(), size);

	if (loadedFont)
	{
		fonts.Add(name, loadedFont);
	}
	else
	{
		Log::Error("Font '%s' could not be loaded at '%s'", name.ToString().c_str(), sPath.c_str());
	}
}

void UIManager::PushFont(Name name)
{
	ImFont** font = fonts.Find(name);
	if (font && *font)
	{
		ImGui::PushFont(*font);
	}
	else
	{
		Log::Error("Font '%s' not found or loaded", name.ToString().c_str());
	}
}

void UIManager::PopFont()
{
	ImGui::PopFont();
}

void UIManager::ApplyStyle()
{
	auto& style = ImGui::GetStyle();

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_FrameBg]         = ImVec4(0.48f, 0.40f, 0.16f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]  = ImVec4(0.98f, 0.77f, 0.26f, 0.40f);
	colors[ImGuiCol_FrameBgActive]   = ImVec4(0.98f, 0.77f, 0.26f, 0.67f);
	colors[ImGuiCol_TitleBgActive]   = ImVec4(0.48f, 0.40f, 0.16f, 1.00f);
	colors[ImGuiCol_CheckMark]       = ImVec4(0.98f, 0.77f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab]      = ImVec4(0.88f, 0.69f, 0.24f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.77f, 0.26f, 1.00f);
	colors[ImGuiCol_Button]          = ImVec4(0.98f, 0.77f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered]   = ImVec4(0.98f, 0.77f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonActive]    = ImVec4(0.98f, 0.65f, 0.06f, 1.00f);
	colors[ImGuiCol_Header]          = ImVec4(0.98f, 0.77f, 0.26f, 0.31f);
	colors[ImGuiCol_HeaderHovered]   = ImVec4(0.98f, 0.77f, 0.26f, 0.80f);
	colors[ImGuiCol_HeaderActive]    = ImVec4(0.98f, 0.77f, 0.26f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.54f, 0.10f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.54f, 0.10f, 1.00f);
	colors[ImGuiCol_ResizeGrip]      = ImVec4(0.98f, 0.77f, 0.26f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.77f, 0.26f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.77f, 0.26f, 0.95f);
	colors[ImGuiCol_Tab]             = ImVec4(0.58f, 0.47f, 0.18f, 0.86f);
	colors[ImGuiCol_TabHovered]      = ImVec4(0.98f, 0.77f, 0.26f, 0.80f);
	colors[ImGuiCol_TabActive]       = ImVec4(0.68f, 0.54f, 0.20f, 1.00f);
	colors[ImGuiCol_TabUnfocused]    = ImVec4(0.15f, 0.13f, 0.07f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.34f, 0.14f, 1.00f);
	colors[ImGuiCol_DockingPreview]  = ImVec4(0.98f, 0.77f, 0.26f, 0.70f);
	colors[ImGuiCol_TextSelectedBg]  = ImVec4(0.98f, 0.77f, 0.26f, 0.35f);
	colors[ImGuiCol_NavHighlight]    = ImVec4(0.98f, 0.77f, 0.26f, 1.00f);

	style.WindowRounding = 2;
	style.ScrollbarRounding = 2;
}
