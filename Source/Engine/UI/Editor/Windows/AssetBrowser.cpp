// Copyright 2015-2019 Piperift - All rights reserved

#include "AssetBrowser.h"
#include "Core/Files/FileSystem.h"


#if WITH_EDITOR

const TAssetPtr<Texture> AssetBrowser::tex {"myTexture.json"};

void AssetBrowser::Build()
{
	Super::Build();
	bOpen = true;
	windowName = TX("Asset Browser");
	windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

	//tex.Load();

	//tex->SetName(TX("MyTexture"));
	//tex->Save();

	auto otherTex = Create<Texture>();
	otherTex->SetName(TX("MyTexture"));
	otherTex->myVector = v3{ 3.f, 4.f, 5.f };
	otherTex->SaveToPath("myOtherTextore.json");
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
