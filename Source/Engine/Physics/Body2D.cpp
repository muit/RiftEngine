// Copyright 2015-2019 Piperift - All rights reserved

#include "Body2D.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>


void Body2D::SetMobility(EMobilityType mobility)
{
	assert(bodyPtr);
	bodyPtr->SetType(b2BodyType(mobility));
}
