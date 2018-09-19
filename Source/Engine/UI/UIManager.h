// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "Object.h"
#include "Object/ObjectPtr.h"
#include "Editor/EditorManager.h"


class UIManager : public Object {
	CLASS(UIManager, Object)

	GlobalPtr<EditorManager> editor;

public:

	void Tick(float deltaTime) {

	}

};
