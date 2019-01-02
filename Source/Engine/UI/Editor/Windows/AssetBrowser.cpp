// Copyright 2015-2019 Piperift - All rights reserved

#include "AssetBrowser.h"
#include "Core/Files/FileSystem.h"
#include "SDL_log.h"


#if WITH_EDITOR

const TAssetPtr<Texture> AssetBrowser::tex {"myTexture.json"};

void AssetBrowser::Build()
{
	Super::Build();
	bOpen = true;
	windowName = TX("Asset Browser");
	windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;


	tex.Load();

	tex->SetName(TX("MyTexture"));
	tex->Save();
}

void AssetBrowser::Tick()
{
	if (bOpen)
	{
		BeginWindow();
		{
			ImGui::Text("Content path: %s", FileSystem::GetAssetsPath().c_str());
			if(tex.IsValid())
				ImGui::Text("Test Value: %s", tex->GetName().ToString().c_str());
			else
				ImGui::Text("Test Value: Not loaded");
		}
		EndWindow();
	}
}

#endif
