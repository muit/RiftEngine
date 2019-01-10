// Copyright 2015-2019 Piperift - All rights reserved

#include "SRenderMesh.h"
#include "Core/Rendering/Commands/ResourceCommands.h"
#include "World/World.h"


void SRenderMesh::Tick(float /*deltaTime*/)
{
	GetWorld()->QueueRender<DrawMeshCommand>(0);
	GetWorld()->QueueRender<DrawMeshCommand>(0);
}
