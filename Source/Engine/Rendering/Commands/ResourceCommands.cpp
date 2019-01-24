// © 2019 Miguel Fernández Arce - All rights reserved

#include "ResourceCommands.h"



void DrawMeshCommand::TransformToWorld(MeshData::VertexBuffer& vertices)
{
	Transform::Matrix toWorld = transform.ToWorldMatrix();

	for (auto& vertex : vertices)
	{
		vertex = toWorld * vertex;
	}
}
