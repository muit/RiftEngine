// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Reflection/Reflection.h"
#include "Object/BaseObject.h"


class Object : public BaseObject {
	CLASS(Object, BaseObject)

private:

	Object();// class Ptr<Object> parent);

	//Ptr<Object> parent;
};
