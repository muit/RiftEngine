// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreStruct.h"

/**
 * Components hold Variables, Getters and Setters. They never have logic
 */
class Component : public Pod {
	STRUCT(Component)

	PROP(uint8, time)
	uint8 time;
};


class Transform : public Component {
	STRUCT(Transform)

	PROP(float, time)
	float time;
};
