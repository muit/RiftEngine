// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Editor/EditorWindow.h"
#include "Editor/Widgets/PropertyWidget.h"
#include "ECS/EntityId.h"


class SceneDetails : public EditorWindow {
	CLASS(SceneDetails, EditorWindow)

	EntityId entity = NoEntity;
	Ptr<Object> object;

protected:

	virtual void Build() override;

	virtual void Tick(float deltaTime) override;

public:

	void SetEntity(EntityId inEntity);
	void SetObject(Ptr<Object> inObject);
};

#endif
