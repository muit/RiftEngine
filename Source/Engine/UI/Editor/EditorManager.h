// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "CoreObject.h"
#include "Editor.h"

#include <imgui/imgui.h>
#include "World/World.h"
#include "UI/Widget.h"
#include "Windows/SceneEntities.h"
#include "Windows/Details.h"
#include "Windows/MemoryDebugger.h"


class EditorManager : public Object {
	CLASS(EditorManager, Object)


	eastl::vector<GlobalPtr<Editor>> activeEditors;

	GlobalPtr<SceneEntities> sceneEntities;
	GlobalPtr<Details> details;
	GlobalPtr<MemoryDebugger> memory;


	bool showDemoWindow = true;
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


public:

	EditorManager() : Super() {
		sceneEntities = Widget::CreateStandalone<SceneEntities>();

		details = Widget::CreateStandalone<Details>();
		details->SetObject(GetWorld()->GetScene());

		memory = Widget::CreateStandalone<MemoryDebugger>();
	}

	void Tick(float deltaTime);

	void TickDocking();
	void TickMainNavBar();


	template<typename EditorType>
	Ptr<Editor> CreateEditor()
	{
		activeEditors.push_back(Create<EditorType>(GetSelf()));
		return activeEditors.back();
	}
};

#endif
