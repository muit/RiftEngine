// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "CCamera.h"


class CEditorCamera : public CCamera {
	STRUCT(CEditorCamera, CCamera)

	PROP(float, forwardSpeed, DetailsEdit)
	float forwardSpeed = 10.f;

	PROP(float, sideSpeed, DetailsEdit)
	float sideSpeed = 10.f;

	PROP(float, rotateSpeed, DetailsEdit)
	float rotateSpeed = 20.f;
};
