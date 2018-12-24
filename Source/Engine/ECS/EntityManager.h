// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <entt/entity/registry.hpp>


using EntityId = uint32;
class Archive;


class EntityManager : public Object {
	CLASS(EntityManager, Object)


	entt::Registry<EntityId> registry;


public:

	EntityId CreateEntity(json data = {})
	{
		return registry.create();
	}

	void DestroyEntity(EntityId entity)
	{
		registry.destroy(entity);
	}

	bool IsValid(EntityId entity) const
	{
		return registry.valid(entity);
	}

private:

	virtual bool Serialize(Archive& ar) override;

	void SerializeEntity(Archive& ar, const EntityId& entity);
};
