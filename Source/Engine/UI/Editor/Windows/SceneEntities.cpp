// Copyright 2015-2019 Piperift - All rights reserved

#include "SceneEntities.h"
#include "Core/Log.h"
#include "Core/Serialization/Archive.h"
#include "World/World.h"
#include "ECS/EntityManager.h"
#include "Gameplay/Components/CEntity.h"


#if WITH_EDITOR

void SceneEntities::Build()
{
	Super::Build();
	windowName = TX("Entities");
	windowFlags = ImGuiWindowFlags_AlwaysAutoResize;
}

void SceneEntities::Tick()
{
	Ptr<World> world = GetWorld();

	if (bOpen && world)
	{
		auto entityManager = world->GetEntityManager();

		if (entityManager)
		{
			BeginWindow();

			filter.Draw("##FilterDraw");

			auto view = entityManager->View<CEntity>();
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
	const i32 selectedIndex = selectedEntities.FindIndex(entity);
	const bool selected = selectedIndex != NO_INDEX;

	if (!selected)
	{
		selectedEntities.Empty();
		selectedEntities.Add(entity);

		// Display the serialized data of an entity
		JsonArchive ar{};
		GetWorld()->GetEntityManager()->SerializeEntity(ar, entity);

		Log::Message(ar.GetDataString().c_str());
	}
}

#endif
