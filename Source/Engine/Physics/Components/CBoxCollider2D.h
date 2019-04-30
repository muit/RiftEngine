// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "CCollider2D.h"


class CBoxCollider2D : public CCollider2D {
	STRUCT(CBoxCollider2D, CCollider2D)

protected:

	PROP(v2, size, DetailsEdit);
	v2 size;


public:

	void SetSize(v2 inSize)
	{
		MarkDirty();
		size = inSize;
	}

	virtual void SetupProperties(Fixture2D::Parameters& params)
	{

	}

	virtual void UpdateProperties()
	{
	}
};
