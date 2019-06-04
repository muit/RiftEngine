// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/CSingleton.h"


/** Contains the path cubes will follow and how many of them will be spawned */
class CSquare : public CSingleton
{
	STRUCT(CSquare, CSingleton)


public:

	PROP(u8, cubeCount, DetailsEdit)
	u8 cubeCount;

	PROP(v3, cornerA, DetailsEdit)
	v3 cornerA;

	PROP(v3, cornerB, DetailsEdit)
	v3 cornerB;

	PROP(v3, cornerC, DetailsEdit)
	v3 cornerC;

	PROP(v3, cornerD, DetailsEdit)
	v3 cornerD;
};
