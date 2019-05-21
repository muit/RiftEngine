// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "ECS/ECSManager.h"
#include "Core/Math/Transform.h"
#include "ECS/ECSManager.h"


class CTransform : public Component {
	STRUCT(CTransform, Component)

public:

	PROP(Transform, transform, DetailsEdit);
	Transform transform{};

	/** Parent entity */
	PROP(EntityId, parent)
	EntityId parent;


	Transform&       GetWorldTransform()       { return transform; }
	const Transform& GetWorldTransform() const { return transform; }

	Transform GetRelativeTransform(const Ptr<ECSManager>& ecs) const;


	/** WORLD GETTERS */
	v3& GetWLocation() { return transform.location; }
	const v3& GetWLocation() const { return transform.location; }

	Quat& GetWRotation() { return transform.rotation; }
	const Quat& GetWRotation() const { return transform.rotation; }

	v3& GetWScale() { return transform.scale; }
	const v3& GetWScale() const { return transform.scale; }

	/** RELATIVE GETTERS */
	v3 GetRLocation(const Ptr<ECSManager>& ecs) const { return GetRelativeTransform(ecs).location; }
	//void SetRLocation(const Ptr<ECSManager>& ecs, const v3& location) const {}

	Quat GetRRotation(const Ptr<ECSManager>& ecs) const { return GetRelativeTransform(ecs).rotation; }

	v3 GetRScale(const Ptr<ECSManager>& ecs) const { return GetRelativeTransform(ecs).scale; }


	/** WORLD SETTERS */
	void SetWLocation(const v3& location) { transform.location = location; }
	void SetWRotation(const Quat& rotation) { transform.rotation = rotation; }
	void SetWScale(const v3& scale) { transform.scale = scale; }
};
