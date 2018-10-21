// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/type_traits.h>
#include <EASTL/shared_ptr.h>

#include "EngineTypes.h"
#include "Reflection/Reflection.h"

class Class;
class Object;


class BaseObject : public eastl::enable_shared_from_this<BaseObject> {

public:

	BaseObject() = default;
	virtual ~BaseObject() {}

	void Destroy() { BeforeDestroy(); }

	Class* GetClass();

protected:

	virtual void BeforeDestroy() {}

};
