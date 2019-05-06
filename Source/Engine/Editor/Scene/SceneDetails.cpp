// Copyright 2015-2019 Piperift - All rights reserved

#include "SceneDetails.h"

#include "World.h"
#include "Core/Reflection/Runtime/HandleHelper.h"
#include "Core/Reflection/ReflectionTags.h"
#include "Core/Object/Object.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"

#include "Widgets/ComponentDetails.h"

#include "Gameplay/Components/CEntity.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Game/Components/CPlayer.h"
#include "Gameplay/Game/Components/CPlatform.h"
#include "Physics/Components/CBody2D.h"
#include "Physics/Components/CBoxCollider2D.h"
#include "Physics/Components/CCircleCollider2D.h"


#if WITH_EDITOR
void SceneDetails::Build()
{
	Super::Build();
	SetName(TX("Details"));

	if (object)
	{
		displayName = String("Details: ").append(object->GetName().ToString());

		if (Class * objectClass = object->GetClass())
		{
			for (const auto& property : objectClass->GetAllProperties())
			{
				if (property.second->HasTag(DetailsEdit) || property.second->HasTag(DetailsView))
				{
					auto handle = property.second->CreateHandle(object);
					Add(PropertyWidget::NewPropertyWidget(Self<Widget>(), handle));
				}
			}
		}
	}
	else if (entity != NoEntity)
	{
		auto ecs = GetWorld()->GetECS();

		AddNew<ComponentDetails<CEntity>>(entity);
		AddNew<ComponentDetails<CTransform>>(entity);
		AddNew<ComponentDetails<CMesh>>(entity);
		AddNew<ComponentDetails<CBody2D>>(entity);
		AddNew<ComponentDetails<CBoxCollider2D>>(entity);
		AddNew<ComponentDetails<CCircleCollider2D>>(entity);
		AddNew<ComponentDetails<CCamera>>(entity);
		AddNew<ComponentDetails<CPlayer>>(entity);
		AddNew<ComponentDetails<CPlatform>>(entity);

		TArray<StructType*> componentTypes;
		Component::StaticStruct()->GetAllChildren(componentTypes);
	}
}

void SceneDetails::Tick(float deltaTime)
{
	if (bOpen)
	{
		BeginWindow();
		if (object || entity != NoEntity)
		{
			TickChilds(deltaTime);
		}
		EndWindow();
	}
}

void SceneDetails::SetEntity(EntityId inEntity)
{
	if (inEntity != entity)
	{
		entity = inEntity;
		object = nullptr;
		ForceRebuild();
	}
}
void SceneDetails::SetObject(Ptr<Object> inObject)
{
	if (inObject != object)
	{
		object = inObject;
		entity = NoEntity;
		ForceRebuild();
	}
}

#endif
