// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <imgui/imgui.h>

#include "Object.h"
#include "Object/ObjectPtr.h"
#include "Editor/EditorManager.h"


class UIManager : public Object {
	CLASS(UIManager, Object)

#if WITH_EDITOR
	GlobalPtr<EditorManager> editor;
#endif

	bool showDemoWindow = true;
	bool showAnotherWindow = false;
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:

	void Prepare();

	void Tick(float deltaTime);
};
