// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <EASTL/functional.h>
#include <EASTL/numeric_limits.h>
#include <entt/entity/entity.hpp>

#include "Core/Platform/Platform.h"
#include "Core/Reflection/ClassTraits.h"
#include "EngineTypes.h"


struct EntityId {
	u32 id;

	EntityId() : id{ eastl::numeric_limits<u32>::max() } {} // Id to NoEntity
	constexpr EntityId(u32 id) : id{ id } {}
	EntityId(entt::internal::null other) : id{ static_cast<u32>(other) } {}
	operator u32() const { return id; }

	friend constexpr bool operator==(EntityId a, EntityId b) { return a.id == b.id; }
	friend constexpr bool operator!=(EntityId a, EntityId b) { return a.id != b.id; }


	bool Serialize(class Archive& ar, const char* name);
};

constexpr EntityId NoEntity{ eastl::numeric_limits<u32>::max() };

DEFINE_CLASS_TRAITS(EntityId,
	HasCustomSerialize = true
);

DECLARE_REFLECTION_TYPE(EntityId);


namespace eastl {
	template<>
	struct hash<EntityId> {
		size_t operator()(EntityId id) const { return static_cast<size_t>(id.id); };
	};
}

namespace entt {
	/**
	 * @brief Entity traits for a EntityId 32bits identifier.
	 *
	 * A 32 bits entity identifier guarantees:
	 *
	 * * 20 bits for the entity number (suitable for almost all the games).
	 * * 12 bit for the version (resets in [0-4095]).
	 */
	template<>
	struct entt_traits<EntityId> {
		/*! @brief Underlying entity type. */
		using entity_type = u32;
		/*! @brief Underlying version type. */
		using version_type = u16;
		/*! @brief Difference type. */
		using difference_type = i64;

		/*! @brief Mask to use to get the entity number out of an identifier. */
		static constexpr u32 entity_mask = 0xFFFFF;
		/*! @brief Mask to use to get the version out of an identifier. */
		static constexpr u32 version_mask = 0xFFF;
		/*! @brief Extent of the entity number within an identifier. */
		static constexpr auto entity_shift = 20;
	};
}
