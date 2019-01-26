// © 2019 Miguel Fernández Arce - All rights reserved

#include "ResourceCommands.h"
#include "Core/Math/Vector.h"


void DrawMeshCommand::TransformToCamera(FrameRender& render, MeshData::VertexBuffer& vertices)
{
	ZoneScoped("TransformToCamera");
	const Matrix4f worldToCamera = render.camera.GetPerspectiveMatrix();
	const Transform::Matrix toWorld = transform.ToWorldMatrix();

	// Viewport transform
	v3 halfScreen{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 0.f };
	const Eigen::Translation3f translateViewport(halfScreen);
	halfScreen.z() = 100000000.f;
	Transform::Matrix toViewport = translateViewport * Scaling(halfScreen);


	Matrix4f finalTransform = toViewport * worldToCamera * toWorld.matrix();

	for (auto& vertex : vertices)
	{
		v4 vertex4; // Not declared outside. Would cause cache misses
		vertex4 << vertex, 1;
		vertex = (finalTransform * vertex4).head<3>();
	}

}

void DrawMeshCommand::TransformToViewport(MeshData::VertexBuffer& vertices)
{
}

void DrawMeshCommand::BackfaceCulling(const MeshData::VertexBuffer& vertices, MeshData::TriangleBuffer& triangles)
{
	ZoneScoped("BackfaceCulling");

	// Remove all triangles that don't look towards the camera
	for (i32 i = 0; i < triangles.Size(); ++i)
	{
		const v3_u32& triangle = triangles[i];

		const v3& v0 = vertices[triangle.x()];
		const v3& v1 = vertices[triangle.y()];
		const v3& v2 = vertices[triangle.z()];

		// Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
		// Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:

		const bool lookingFront = ((v1.x() - v0.x()) * (v2.y() - v0.y()) - (v2.x() - v0.x()) * (v1.y() - v0.y()) > 0.f);

		if (!lookingFront)
		{
			triangles.RemoveAtSwap(i);
			--i;
		}
	}
}

void DrawMeshCommand::RenderTriangles(FrameRender& render, const MeshData::VertexBuffer& vertices, const MeshData::TriangleBuffer& triangles)
{
	ZoneScoped("RasterizeTriangles");
	render.rasterizer.FillVertexBuffer(vertices, triangles, Color::Blue);
}
