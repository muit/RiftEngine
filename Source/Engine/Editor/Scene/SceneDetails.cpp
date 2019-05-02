// Copyright 2015-2019 Piperift - All rights reserved

#include "SceneDetails.h"

#include "World.h"
#include "Core/Reflection/Runtime/HandleHelper.h"
#include "Core/Reflection/ReflectionTags.h"
#include "Core/Object/Object.h"
#include "ECS/ECSManager.h"
#include "ECS/Component.h"


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

		TArray<StructType*> componentTypes;
		Component::StaticStruct()->GetAllChildren(componentTypes);

		/*for (const auto* type : componentTypes)
		{
			for (const auto& property : type->GetAllProperties())
			{
				if (property.second->HasTag(DetailsEdit) || property.second->HasTag(DetailsView))
				{
					auto handle = property.second->CreateHandle(object);
					Add(PropertyWidget::NewPropertyWidget(handle));
				}
			}
		}*/
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
