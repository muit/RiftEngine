// Copyright 2015-2019 Piperift - All rights reserved

#include "CTransform.h"

Transform CTransform::GetRelativeTransform(const Ptr<ECSManager>& ecs) const
{
	assert(ecs);

	if (ecs->IsValid(parent))
	{
		if (ecs->Has<CTransform>(parent))
		{
			// Return relative transform from parent's world transform
			return ecs->Get<CTransform>(parent).transform * transform;
		}
		else
		{
			Log::Error("Parent entity doesn't have a transform component!");
		}
	}
	// No valid parent, return global
	return GetWorldTransform();
}
