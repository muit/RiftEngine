// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "CCamera.h"


class CEditorCamera : public CCamera {
	STRUCT(CEditorCamera, CCamera)

	PROP(float, forwardSpeed, DetailsEdit | Transient)
	float forwardSpeed = 15.f;

	PROP(float, sideSpeed, DetailsEdit | Transient)
	float sideSpeed = 15.f;

	PROP(float, rotateSpeed, DetailsEdit | Transient)
	float rotateSpeed = 45.f;
};
