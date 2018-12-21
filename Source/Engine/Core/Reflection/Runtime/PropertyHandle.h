// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Core/Object/BaseObject.h"
#include "Core/Object/ObjectPtr.h"


#if WITH_EDITOR
class PropertyWidget;
#endif


struct PropertyHandle {
protected:

	PropertyHandle() {}

public:
	virtual ~PropertyHandle() {}

	virtual bool IsValid() const = 0;
};
