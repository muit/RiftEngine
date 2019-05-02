// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "Rendering/Data/CameraData.h"


class CActiveCamera : public Component {
	STRUCT(CActiveCamera, Component)

	CameraData activeData;
};
