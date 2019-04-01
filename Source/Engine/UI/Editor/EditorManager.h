// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "CoreObject.h"
#include "Editor.h"

#include <imgui/imgui.h>
#include "Core/World.h"
#include "UI/Widget.h"
#include "Windows/SceneEntities.h"
#include "Windows/Details.h"
#include "Windows/MemoryDebugger.h"
#include "Windows/AssetBrowser.h"


class EditorManager : public Object {
	CLASS(EditorManager, Object)


	eastl::vector<GlobalPtr<Editor>> activeEditors;

	GlobalPtr<SceneEntities> sceneEntities;
	GlobalPtr<Details> details;
	GlobalPtr<MemoryDebugger> memory;
	GlobalPtr<AssetBrowser> assetBrowser;


	bool showDemoWindow = true;

public:

	EditorManager() : Super() {

		details = Widget::CreateStandalone<Details>();
		details->SetObject(GetWorld()->GetActiveScene().Get());

		sceneEntities = Widget::CreateStandalone<SceneEntities>();
		sceneEntities->sceneDetailsWindow = details;

		memory = Widget::CreateStandalone<MemoryDebugger>();
		assetBrowser = Widget::CreateStandalone<AssetBrowser>();
	}

	void Tick(float deltaTime);

	void TickDocking();
	void TickMainNavBar();


	template<typename EditorType>
	Ptr<Editor> CreateEditor()
	{
		activeEditors.push_back(Create<EditorType>(Self()));
		return activeEditors.back();
	}
};

#endif
