// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"


class System : public Object {
	CLASS(System, Object)

public:

	virtual void BeginPlay() {}
	virtual void Tick(float /*deltaTime*/) {}
	virtual void EndPlay() {}
};
