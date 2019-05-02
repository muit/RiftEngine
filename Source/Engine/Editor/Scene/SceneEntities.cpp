// Copyright 2015-2019 Piperift - All rights reserved

#include "SceneEntities.h"
#include "Core/Log.h"
#include "Core/Serialization/Archive.h"
#include "World.h"
#include "ECS/ECSManager.h"
#include "Gameplay/Components/CEntity.h"
#include "Core/Engine.h"


#if WITH_EDITOR

void SceneEntities::Build()
{
	Super::Build();
	idName = TX("Entities");
	windowFlags = ImGuiWindowFlags_AlwaysAutoResize;
}

void SceneEntities::Tick()
{
	Ptr<World> world = GetWorld();

	if (bOpen && world)
	{
		auto ecsManager = world->GetECS();

		if (ecsManager)
		{
			BeginWindow();

			filter.Draw("##FilterDraw");

			auto view = ecsManager->View<CEntity>();
			i32 numOfEntities = (i32)view.size();

			if (ImGui::ListBoxHeader("##EntityContainer", numOfEntities, 10))
			{
				ImGuiListClipper clipper(numOfEntities, ImGui::GetTextLineHeightWithSpacing());
				while (clipper.Step())
				{
					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
					{
						EntityId entity = *(view.cbegin() + i);
						String entName = view.get(entity).name.ToString();

						ImGui::PushID(i);
						if (filter.PassFilter(entName.begin(), entName.end()))
						{
							const bool selected = selectedEntities.Contains(entity);
							if (ImGui::Selectable(entName.c_str(), selected))
							{
								OnEntityClicked(entity);
							}
						}
						ImGui::PopID();
					}
				}
				ImGui::ListBoxFooter();
			}
			EndWindow();
		}
	}
}

void SceneEntities::OnEntityClicked(EntityId entity)
{
	auto input = GEngine->GetInput();

	const i32 index = selectedEntities.FindIndex(entity);
	if (index == NO_INDEX)
	{
		// If Ctrl is pressed, add the entity to the list
		if (!input->IsModifierPressed(EKeyModifier::Ctrl))
		{
			selectedEntities.Empty();
		}
		selectedEntities.Add(entity);
	}
	else
	{
		// Already selected select
		if (!input->IsModifierPressed(EKeyModifier::Ctrl))
		{
			selectedEntities.Empty();
		}
		else
		{
			selectedEntities.RemoveAt(index);
		}
	}

	onSelectionChanged.DoBroadcast(selectedEntities);
}

#endif
