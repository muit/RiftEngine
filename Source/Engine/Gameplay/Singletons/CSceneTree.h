// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/EntityId.h"
#include "ECS/Component.h"

struct SceneTreeNode {
	EntityId entity;

	// Children by index
	TArray<i32> children;
};

// #TODO: Improve this awful std syntax
namespace eastl {
	template<>
	struct hash<SceneTreeNode>
	{
		size_t operator()(const SceneTreeNode& node) const {
			return hash<EntityId>{}(node.entity);
		}
	};
};


class CSceneTree : public Component {
	STRUCT(CSceneTree, Component)

	TArray<SceneTreeNode> rootEntities;

	TArray<SceneTreeNode> nonRootEntities;


	TArray<EntityId> GetAllChildren(EntityId entity);
};


