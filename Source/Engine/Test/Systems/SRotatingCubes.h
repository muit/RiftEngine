// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "ECS/System.h"
#include "ECS/ECSManager.h"


class SRotatingCubes : public System {
	CLASS(SRotatingCubes, System)

	class CSquare* squareComp;


public:

	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

private:

	void Update() {}

	v3 GetSegmentPosition(u8 segment) const;
};
