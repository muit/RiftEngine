// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>


enum class EMobilityType : u8
{
	Static,
	Kinematic,
	Movable
};

enum class EShapeType : u8
{
	Box,
	Circle,
	Geometry
};

using Shape = b2Shape;
using PolygonShape = b2PolygonShape;
using CircleShape = b2CircleShape;
