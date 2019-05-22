// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"

#include <imgui/imgui.h>
#include <SDL_events.h>

#include "Core/Files/FileSystem.h"

#include "Core/Object/ObjectPtr.h"
#include "Editor/EditorManager.h"
#include "Core/Containers/Map.h"


class UIManager : public Object {
	CLASS(UIManager, Object)

#if WITH_EDITOR
	GlobalPtr<EditorManager> editor;
#endif

	static TMap<Name, ImFont*> fonts;


public:

	UIManager() : Super() {
		fonts.SetEmptyKey("");
	}

	void Prepare();

	void OnSDLEvent(SDL_Event* ev);

	void Tick(float deltaTime);

	static void AddFont(Name name, Path path, u8 size = 14u);
	static void PushFont(Name name);
	static void PopFont();

private:

	void ApplyStyle();
};
