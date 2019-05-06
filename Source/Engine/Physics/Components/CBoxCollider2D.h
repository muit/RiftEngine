// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "../PhysicsTypes.h"
#include "CCollider2D.h"


/**
 * A Box collider is a collider with a box shape.
 * @see CCollider2D
 */
class CBoxCollider2D : public CCollider2D {
	STRUCT(CBoxCollider2D, CCollider2D)

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

	virtual void FillDefinition(b2FixtureDef& def, Shape* shape) const override
	{
		CCollider2D::FillDefinition(def, shape);

		auto* pShape = static_cast<PolygonShape*>(shape);
		pShape->SetAsBox(size.x, size.y);
	}
};
