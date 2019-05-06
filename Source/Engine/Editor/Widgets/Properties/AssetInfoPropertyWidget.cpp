// Copyright 2015-2019 Piperift - All rights reserved

#include "AssetInfoPropertyWidget.h"

#if WITH_EDITOR
#include <imgui/imgui.h>
#include <imgui/imgui_stl.h>
#include "Core/Reflection/ReflectionTags.h"


void AssetInfoPropertyWidget::Tick(float)
{
	ImGuiInputTextFlags flags = 0;
	if (!prop->HasTag(DetailsEdit))
		flags |= ImGuiInputTextFlags_ReadOnly;

	ImGui::PushID(GetName().ToString().c_str());
	{
		if (auto* infoHandle = GetInfoHandle())
		{
			EditAssetInfo(*infoHandle->GetValuePtr());
		}
		else if (auto* assetPtr = GetAssetPtrValuePtr())
		{
			EditAssetInfo(assetPtr->info);
		}

	}
	ImGui::PopID();
}

void AssetInfoPropertyWidget::EditAssetInfo(AssetInfo& info)
{
	String id = info.GetPath().ToString();
	ImGui::InputText(displayName.c_str(), id);

	if (id != info.GetPath().ToString())
	{
		info = { Name{id} };
	}
}

#endif

