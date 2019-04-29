// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Math/Vector.h"

#include "Physics2D.h"


enum class EPhysicsMode : u8
{
	Mode2D,
	Mode3D
};


class PhysicsManager : public Object
{
	CLASS(PhysicsManager, Object)

	GlobalPtr<Physics2D> physics2D;

public:

	void Initialize();
	void Tick(float deltaTime);

	Ptr<Physics2D> Get2D() const { physics2D; }
};
