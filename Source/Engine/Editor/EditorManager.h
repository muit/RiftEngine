// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "Object.h"
#include "Editor.h"

#if WITH_EDITOR

class EditorManager : public Object {
	CLASS(EditorManager, Object)

	std::vector<GlobalPtr<Editor>> activeEditors;

public:

	template<typename EditorType>
	Ptr<Editor> CreateEditor()
	{
		activeEditors.push_back(Create<EditorType>(ThisPtr()));
		return activeEditors.back().GetPtr();
	}
};

#endif
