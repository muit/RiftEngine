// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Editor/EditorWindow.h"

#include "Core/Assets/AssetPtr.h"
#include "Assets/Texture.h"


class AssetBrowser : public EditorWindow {
	CLASS(AssetBrowser, EditorWindow)

	static const TAssetPtr<Texture> tex;
	json testFile;


protected:

	virtual void Build() override;

	virtual void Tick() override;
};

#endif
