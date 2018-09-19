// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "Object.h"
#include "EditorWindow.h"

#if WITH_EDITOR

class Editor : public Object {
    CLASS(Editor, Object)

    std::vector<GlobalPtr<EditorWindow>> Windows;
};

#endif
