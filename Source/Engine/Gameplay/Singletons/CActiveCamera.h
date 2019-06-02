// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/CSingleton.h"
#include "Rendering/Data/CameraData.h"


class CActiveCamera : public CSingleton {
	STRUCT(CActiveCamera, CSingleton, Transient)

	CameraData activeData;
};
