// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "CCamera.h"


class CEditorCamera : public CCamera {
	STRUCT(CEditorCamera, CCamera)

	PROP(float, fov)
	float fov = 60;
};
