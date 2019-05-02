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

	Ptr<SceneEditor> sceneEditor;


	bool showDemoWindow = true;

public:

	virtual void Construct() override
	{
		Super::Construct();

		sceneEditor = CreateEditor<SceneEditor>();

		memory = Widget::CreateStandalone<MemoryDebugger>(Self());
		assetBrowser = Widget::CreateStandalone<AssetBrowser>(Self());
	}

	void Tick(float deltaTime);

	void TickDocking();
	void TickMainNavBar();


	template<typename EditorType>
	Ptr<EditorType> CreateEditor()
	{
		const i32 i = editors.Add(Create<EditorType>(Self()));
		return editors[i].AsPtr().Cast<EditorType>();
	}
};

#endif
