// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "CCollider.h"
#include "../PhysicsTypes.h"


/**
 * A Sphere collider is a collider with a circular shape.
 * @see CCollider
 */
class CSphereCollider : public CCollider {
	STRUCT(CSphereCollider, CCollider)

protected:

	PROP(float, radius, DetailsEdit);
	float radius = 0.5f;


public:

	void SetRadius(float inRadius)
	{
		MarkDirty();
		radius = inRadius;
	}
};
