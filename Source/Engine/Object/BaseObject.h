// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/type_traits.h>

#include "EngineTypes.h"
#include "Reflection/Reflection.h"

class Class;
class Object;

template<typename Type>
class Ptr;


class BaseObject {

public:

	BaseObject() = default;
	virtual ~BaseObject() {}

	void Destroy() { BeforeDestroy(); }

	Class* GetClass() const;
	Ptr<Object> GetSelf() const;

protected:

	virtual void BeforeDestroy() {}

};
