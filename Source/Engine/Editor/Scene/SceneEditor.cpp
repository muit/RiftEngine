// Copyright 2015-2019 Piperift - All rights reserved

#include "SceneEditor.h"

#if WITH_EDITOR
#include "World.h"


void SceneEditor::Construct()
{
	Super::Construct();

	sceneEntities = Widget::CreateStandalone<SceneEntities>();
	sceneEntities->onSelectionChanged.Bind(this, &SceneEditor::OnSelectionChanged);

	details = Widget::CreateStandalone<SceneDetails>();

	// Select scene
	details->SetObject(GetWorld()->GetActiveScene().Get());
}

void SceneEditor::ExpandViewsMenu()
{
	if (ImGui::MenuItem("Entities", (const char*)0, sceneEntities->IsOpenedPtr())) {}
	if (ImGui::MenuItem("Details", (const char*)0,  details->IsOpenedPtr())) {}
}

void SceneEditor::OnSelectionChanged(const TArray<EntityId>& entities)
{
	selections = entities;
	if (selections.Size() > 0)
	{
		details->SetEntity(selections[0]);
	}
	else
	{
		details->SetObject(GetWorld()->GetActiveScene().Get());
	}
}

#endif
