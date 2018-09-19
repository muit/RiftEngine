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

public:

	void Prepare();

	void Tick(float deltaTime);
};
