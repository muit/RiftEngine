// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "CoreObject.h"
#include "Editor.h"

#include <imgui/imgui.h>

#include "UI/Widget.h"
#include "Windows/MemoryDebugger.h"
#include "Windows/AssetBrowser.h"
#include "Scene/SceneEditor.h"


class EditorManager : public Object {
	CLASS(EditorManager, Object)


	TArray<GlobalPtr<Editor>> editors;

	GlobalPtr<MemoryDebugger> memory;
	GlobalPtr<AssetBrowser> assetBrowser;


	bool showDemoWindow = true;

public:

	EditorManager() : Super()
	{
		CreateEditor<SceneEditor>();

		memory = Widget::CreateStandalone<MemoryDebugger>();
		assetBrowser = Widget::CreateStandalone<AssetBrowser>();
	}

	void Tick(float deltaTime);

	void TickDocking();
	void TickMainNavBar();


	template<typename EditorType>
	Ptr<Editor> CreateEditor()
	{
		editors.Add(Create<EditorType>(Self()));
		return editors.Last();
	}
};

#endif
