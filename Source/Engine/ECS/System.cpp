// Copyright 2015-2019 Piperift - All rights reserved

#include "System.h"
#include "ECSManager.h"


Ptr<ECSManager> System::ECS() const
{
	return GetOwner().Cast<ECSManager>();
}