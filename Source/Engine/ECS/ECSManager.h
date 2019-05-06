// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "eastl/unique_ptr.h"
#include <entt/entt.hpp>

#include "EntityId.h"
#include "Component.h"
#include "System.h"

#include "Gameplay/Components/CEntity.h"
#include "Core/Events/Broadcast.h"


//DECLARE_REFLECTION_TYPE(EntityId);


class Archive;

class ECSManager : public Object {
	CLASS(ECSManager, Object)

	using Registry = entt::registry<EntityId>;

	Registry registry;

	/** List of Guids pointing to entity Ids */
	eastl::unordered_map<Guid, EntityId> guidEntityCache;

	TArray<GlobalPtr<System>> systems;

	TArray<eastl::unique_ptr<Component>> singletonComponents;

public:

	void Initialize()
	{
		RegistrySingletons();
		RegistrySystems();
	}

	void BeginPlay();

	void Tick(float deltaTime);

	void EndPlay()
	{
		IterateSystems([](Ptr<System> system) {
			system->EndPlay();
		});
	}

	void Shutdown() {
		systems.Empty(false);
	}


	/**************************************************************
	 * Begin ENTITIES
	 */

	EntityId CreateEntity(Name entityName, bool bTransient = false);
	void DestroyEntity(EntityId entity);

	// Internal versions that don't track cached Guids
	EntityId __CreateEntity(Name entityName, bool bTransient = false);
	void __DestroyEntity(EntityId entity);


	bool IsValid(EntityId entity) const
	{
		return registry.valid(entity);
	}


	/**************************************************************
	 * Begin SERIALIZATION
	 */

	virtual bool Serialize(Archive& ar) override;

	void SerializeEntity(Archive& ar, EntityId entity);

private:

	template<typename CompType>
	void SerializeComponent(Archive& ar, const EntityId& entity) {
		static_assert(eastl::is_convertible< CompType, Component >::value, "Type is not a Component!");

		if (ar.IsLoading())
		{
			ar.BeginObject(CompType::StaticStruct()->GetSName());
			if(ar.IsObjectValid())
			{
				CompType& comp = Assign<CompType>(entity);
				comp.SerializeReflection(ar);

				if constexpr (ClassTraits<CompType>::HasPostSerialize)
					comp.PostSerialize(true);
			}
			ar.EndObject();
		}
		// Saving
		else if (Has<CompType>(entity))
		{
			ar.BeginObject(CompType::StaticStruct()->GetSName());
			{
				CompType& comp = Get<CompType>(entity);
				comp.SerializeReflection(ar);

				if constexpr (ClassTraits<CompType>::HasPostSerialize)
					comp.PostSerialize(false);
			}
			ar.EndObject();
		}
	}

public:

	const eastl::unordered_map<Guid, EntityId>& GetGuidCache() const { return guidEntityCache; }

	/**************************************************************
	 * Begin SYSTEMS
	 */

	void IterateSystems(eastl::function<void(Ptr<System>)> callback) const {
		for (const GlobalPtr<System>& system : systems)
		{
			if (system)
			{
				callback(system);
			}
		}
	}

	template<typename... Components>
	auto View() {
		return registry.view<Components...>();
	};

	template<typename C, typename... Args>
	C& Assign(EntityId entity, Args... args)
	{
		if (Has<C>(entity))
			return registry.replace<C>(entity, eastl::forward<Args>(args)...);

		return registry.assign<C>(entity, eastl::forward<Args>(args)...);
	}

	template<typename C>
	void Remove(EntityId entity)
	{
		registry.remove<C>(entity);
	}

	template<typename Component>
	Component& Get(const EntityId entity) {
		return registry.get<Component>(entity);
	};

	template<typename Component>
	const Component& Get(const EntityId entity) const {
		return registry.get<Component>(entity);
	};

	template<typename C>
	FORCEINLINE bool Has(EntityId entity) const { return registry.has<C>(entity); }

	u32 GetEntityCount() {
		return (i32)registry.size();
	};

private:

	void RegistrySystems();

	template <typename T>
	void RegistrySystem() {
		systems.Add(Create<T>(Self()));
	}


public:

	/**************************************************************
	 * Begin COMPONENT SINGLETONS
	 * Singleton components are global and shared between all systems
	 */

	void RegistrySingletons();

	template <typename C>
	void RegistrySingleton() {
		singletonComponents.Add(eastl::make_unique<C>());
	}

	template<typename C>
	C* FindSingleton() {
		static_assert(eastl::is_convertible<C, Component>::value, "Type is not a Component!");

		const auto* foundPtr = singletonComponents.Find([](const auto& comp) {
			return comp->GetStruct() == C::StaticStruct();
		});

		return foundPtr ? static_cast<C*>(foundPtr->get()) : nullptr;
	}

	template<typename C>
	bool HasSingleton() {
		static_assert(eastl::is_convertible<C, Component>::value, "Type is not a Component!");

		return singletonComponents.Contains([](const auto& comp) {
			return comp->GetStruct() == C::StaticStruct();
		});
	}


private:

	Broadcast<EntityId> onEntityCreated;
	Broadcast<EntityId> onEntityDestroyed;

public:

	const Broadcast<EntityId>& OnEntityCreated() const { return onEntityCreated; }
	const Broadcast<EntityId>& OnEntityDestroyed() const { return onEntityDestroyed; }
};
