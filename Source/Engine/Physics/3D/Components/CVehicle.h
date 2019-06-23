// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/Component.h"
#include "PxRigidActor.h"
#include "PxRigidDynamic.h"

#include "../../PhysicsTypes.h"
#include "../SPhysics.h"


/** An entity will obtain physics when having a body.
 * The body specifies physical properties of the entity
 * like gravity, mobility type or fixed rotation.
 */
class CVehicle : public Component {
	STRUCT(CVehicle, Component)


public:

	PROP(float, mass, DetailsEdit)
	float mass = 1.f;

	CVehicle() = default;
};
