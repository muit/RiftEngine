// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Editor/EditorWindow.h"
#include "Editor/Widgets/PropertyWidget.h"

#include "ECS/ECSManager.h"


class SceneEntities : public EditorWindow {
	CLASS(SceneEntities, EditorWindow)

	ImGuiTextFilter filter;

	TArray<EntityId> selectedEntities;

public:

	Broadcast<const TArray<EntityId>&> onSelectionChanged;


	SceneEntities() : Super(), filter{} {}

protected:

	virtual void Build() override;

	virtual void Tick() override;

	void OnEntityClicked(EntityId entity);
};

#endif
