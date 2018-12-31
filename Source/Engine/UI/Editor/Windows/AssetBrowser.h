// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Editor/EditorWindow.h"


class AssetBrowser : public EditorWindow {
	CLASS(AssetBrowser, EditorWindow)

	json testFile;

protected:

	virtual void Build() override;

	virtual void Tick() override;
};

#endif
