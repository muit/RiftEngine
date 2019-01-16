// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include <entt/entity/registry.hpp>

#include "Gameplay/Components/CEntity.h"


using EntityId = u32;
class Archive;


class EntityManager : public Object {
	CLASS(EntityManager, Object)


	entt::Registry<EntityId> registry;


public:

	EntityId CreateEntity(Name entityName)
	{
		EntityId ent = registry.create();
		registry.assign<CEntity>(ent, entityName);
		return ent;
	}

	void DestroyEntity(EntityId entity)
	{
		registry.destroy(entity);
	}

	bool IsValid(EntityId entity) const
	{
		return registry.valid(entity);
	}

	template<typename... Component>
	auto View() {
		return registry.view<Component...>();
	};


	virtual bool Serialize(Archive& ar) override;

	void SerializeEntity(Archive& ar, const EntityId& entity);

private:

	template<typename CompType>
	void SerializeComponent(Archive& ar, const EntityId& entity) {
		static_assert(eastl::is_convertible< CompType, Component >::value, "Type is not a Component!");
		if (registry.has<CompType>(entity))
		{
			ar.BeginObject(CompType::StaticStruct()->GetSName());
			{
				CompType& comp = registry.get<CompType>(entity);
				comp.SerializeReflection(ar);
			}
			ar.EndObject();
		}
	}
};
