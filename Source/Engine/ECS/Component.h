// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreStruct.h"

/**
 * Components hold Variables, Getters and Setters. They never have logic
 */
class Component : public Struct {
	STRUCT(Component, Struct)
};
