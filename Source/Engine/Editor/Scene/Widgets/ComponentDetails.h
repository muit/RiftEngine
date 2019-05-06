// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Widget.h"
#include "ECS/ECSManager.h"
#include "Gameplay/Components/CEntity.h"


template<typename Type>
class ComponentDetails : public Widget {
	CLASS(ComponentDetails<Type>, Widget)
public:

	Ptr<ECSManager> ecs;
	EntityId entity = NoEntity;

	void Configure(EntityId inEntity)
	{
		entity = inEntity;
		ecs = GetWorld()->GetECS();
	}

protected:

	virtual void Build() override;
	virtual void Tick(float) override;
};

template<typename Type>
void ComponentDetails<Type>::Build()
{
	if (ecs->Has<Type>(entity))
	{
		Type& comp = ecs->Get<Type>(entity);

		// Display each component property
		for (const auto& property : Type::StaticStruct()->GetAllProperties())
		{
			if (property.second->HasTag(DetailsEdit) || property.second->HasTag(DetailsView))
			{
				auto handle = property.second->CreateHandle(&comp);
				Add(PropertyWidget::NewPropertyWidget(Self<Widget>(), handle));
			}
		}
	}
}

template<typename Type>
void ComponentDetails<Type>::Tick(float deltaTime)
{
	static const Name compName{ Type::StaticStruct()->GetName() };

	if (ecs->IsValid(entity))
	{
		if (ecs->Has<Type>(entity))
		{
			bool bNotCollapsed = ImGui::CollapsingHeader(compName.ToString().c_str(), ImGuiTreeNodeFlags_AllowOverlapMode);

			if(!eastl::is_convertible<Type, CEntity>::value)
			{
				ImGui::SameLine(ImGui::GetWindowWidth() - 30);

				//Remove button. Won't appear on CEntity
				if (ImGui::Button("X"))
				{
					ecs->Remove<Type>(entity);
					bNotCollapsed = false;
				}
			}

			if(bNotCollapsed)
			{
				// Show properties
				TickChilds(deltaTime);
			}
		}
		else
		{
			String bttnText = CString::Printf("Add %s", compName.ToString().c_str());
			if (ImGui::Button(bttnText.c_str()))
			{
				// Assign component
				ecs->Assign<Type>(entity);
				ForceRebuild();
			}
		}
	}
}

#endif
