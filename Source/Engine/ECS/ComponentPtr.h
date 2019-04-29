// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

/**
 * Points to another component
 */
template<typename ComponentType>
class TComponentPtr {
	STRUCT(Component, Struct)

	PROP(EntityId, entity)
	EntityId entity;
};
