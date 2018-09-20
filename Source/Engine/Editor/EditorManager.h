// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Object.h"
#include "Editor.h"

#include <imgui/imgui.h>
#include "UI/Widget.h"
#include "Widgets/Details.h"


class EditorManager : public Object {
	CLASS(EditorManager, Object)


	std::vector<GlobalPtr<Editor>> activeEditors;

	GlobalPtr<Details> details;

	bool showDemoWindow = true;
	bool showAnotherWindow = false;
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


public:

	EditorManager() : Super() {
		details = Widget::CreateStandalone<Details>();
	}

	template<typename EditorType>
	Ptr<Editor> CreateEditor()
	{
		activeEditors.push_back(Create<EditorType>(ThisPtr()));
		return activeEditors.back().GetPtr();
	}

	void Tick(float deltaTime);
};

#endif
