// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "Rendering/Data/CameraData.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Components/CTransform.h"


class SRenderCamera : public System {
	CLASS(SRenderCamera, System)

	virtual void Tick(float deltaTime) override;

private:

	CameraData GetCameraData(const Transform& transform, const CCamera* camera) const {
		CameraData data {};
		data.fov = camera->fov;
		data.transform = transform;
		return data; // Copy elision here
	}
};
