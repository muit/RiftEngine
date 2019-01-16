// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Core/Object/BaseObject.h"
#include "Core/Object/ObjectPtr.h"

#include "ClassHandle.h"


class HandleHelper {
public:

	static eastl::shared_ptr<ClassHandle> CreateClassHandle(const Ptr<BaseObject>& instance)
	{
		return eastl::make_shared<ClassHandle>(instance);
	}
};
