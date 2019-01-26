// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"

#include "Broadcast.h"


class EventManager : public Object {
	CLASS(EventManager, Object)

private:

	eastl::unordered_map<Name, eastl::unique_ptr<BaseBroadcast>> broadcasts;


public:

	void Broadcast();
};
