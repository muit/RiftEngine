// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "ComponentRegistry.h"


class BaseView {};

template<typename Comp, typename... Comps>
class ComponentView : public BaseView
{
	ComponentRegistry* registry;

public:

	ComponentView(ComponentRegistry& registry) : registry{ &registry } {}

	void Each(eastl::function<void(EntityId, Comp&, Comps&...)> callback)
	{
		ComponentPool<Comp>* pool = registry->FindPool<Comp>();
		TTuple<ComponentPool<Comps>* ...> pools{ registry->FindPool<Comps>()... };

		for (const auto& entry : pool->components)
		{
			callback(entry.first, entry.second);
		}
	}

	template<GetComp>
	Comp* Get(EntityId entity)
	{
		static_assert((TIsSame<GetComp, Comp>::value || eastl::disjunction_v<TIsSame<GetComp, Comps>...>) &&
			"Views can only get their owned components");

		return nullptr;
	}
};
