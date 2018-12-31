// Copyright 2015-2019 Piperift - All rights reserved

#include "AssetBrowser.h"
#include "Core/Files/FileSystem.h"


#if WITH_EDITOR

void AssetBrowser::Build()
{
	Super::Build();
	bOpen = true;
	windowName = TX("Asset Browser");
	windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
}

void AssetBrowser::Tick()
{
	if (bOpen)
	{
		BeginWindow();
		{
			ImGui::Text("Content path: %s", FileSystem::GetContentPath().c_str());
		}
		EndWindow();
	}
}

#endif
