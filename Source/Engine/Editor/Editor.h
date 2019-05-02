// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"

#include <EASTL/vector.h>

#include "EditorWindow.h"
#include "Scene/Gizmo.h"


#if WITH_EDITOR

class Editor : public Object {
	CLASS(Editor, Object)

public:

	virtual void Tick(float /*deltaTime*/) {}

	virtual void ExpandViewsMenu() {}
};

#endif
