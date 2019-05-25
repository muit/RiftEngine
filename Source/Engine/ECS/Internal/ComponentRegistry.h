// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "Core/Reflection/StructType.h"
#include "ComponentPool.h"


class ComponentRegistry
{

	using PoolsContainer = TMap<StructType*, eastl::unique_ptr<BaseComponentPool>>;

	// Component Types get registered globally here
	static PoolsContainer defaultPools;

	PoolsContainer pools{ nullptr };


public:

	ComponentRegistry()
	{
		// Instantiate pools based on registry
		pools.Resize(defaultPools.Size());
		for (const auto& pool : defaultPools)
		{
			pools.Insert(pool.first, pool.second->Duplicate());
		}
	};

	BaseComponentPool* FindPool(StructType* type) const {
		const auto* pool = pools.Find(type);
		return pool? pool->get() : nullptr;
	}

	template<typename Component>
	ComponentPool<Component>* FindPool() const {
		return static_cast<ComponentPool<Component>*>(FindPool(Component::StaticStruct()));
	}


	// Called by reflection to registry new component types
	template<typename Component>
	static void RegistryPool()
	{
		defaultPools.Insert(
			Component::StaticStruct(),
			MakeUnique<ComponentPool<Component>>()
		);
	}
};
