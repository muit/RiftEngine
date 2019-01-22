// © 2019 Miguel Fernández Arce - All rights reserved

#include "ResourceCommands.h"



void DrawMeshCommand::TransformToWorld(MeshData::VertexBuffer& vertices)
{
	/*for (auto& vertex : vertices)
	{
		vertex = transform.GetTransformationMatrix() * vertex;
	}*/

	v3 vertex = transform.GetTransformationMatrix() * v3::Ones();
}

void DrawMeshCommand::TransformToCamera(MeshData::VertexBuffer& vertices)
{

}

void DrawMeshCommand::TransformToScreen(MeshData::VertexBuffer& vertices)
{

}

void DrawMeshCommand::BackFaceCulling(MeshData::VertexBuffer& vertices, MeshData::TriangleBuffer& triangles)
{

}

void DrawMeshCommand::Draw(MeshData::VertexBuffer& vertices, MeshData::TriangleBuffer& triangles, Color color)
{

}

