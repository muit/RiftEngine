// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "../EntityId.h"


class BaseComponentPool {

public:

	virtual eastl::unique_ptr<BaseComponentPool> Duplicate() = 0;
};

template<typename ComponentType>
class ComponentPool : BaseComponentPool {

	TMap<EntityId, ComponentType> components{ NoEntity };


public:

	virtual eastl::unique_ptr<BaseComponentPool> Duplicate() override {
		return MakeUnique(new ComponentPool(*this));
	}
};
