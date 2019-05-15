// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "CCollider.h"
#include "../PhysicsTypes.h"


/**
 * A Box collider is a collider with a box shape.
 * @see CCollider
 */
class CBoxCollider : public CCollider {
	STRUCT(CBoxCollider, CCollider)

protected:

	PROP(v2, offset, DetailsEdit);
	v2 offset;

	PROP(v2, size, DetailsEdit);
	v2 size {1.f, 1.f};


public:

	void SetSize(v2 inSize)
	{
		MarkDirty();
		size = inSize;
	}
};
