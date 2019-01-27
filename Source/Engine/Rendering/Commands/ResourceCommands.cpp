// © 2019 Miguel Fernández Arce - All rights reserved

#include "ResourceCommands.h"
#include "Core/Math/Vector.h"


void DrawMeshCommand::Execute(FrameRender& render, Frame& frame)
{
	const MeshData& mesh = render.resources.Get<ResourceType::Mesh>(id);

	TriangleBuffer triangles{ mesh.GetTriangles() };

	VertexBufferI32 vertices{ (u32)mesh.GetVertices().Size() };
	TransformToScreen(render, mesh.GetVertices(), vertices);

	BackfaceCulling(vertices, triangles);
	RenderTriangles(render, vertices, triangles);
}

void DrawMeshCommand::TransformToScreen(FrameRender& render, const VertexBuffer& vertices, VertexBufferI32& outVertices)
{
	ZoneScoped("TransformToCamera");

	const Matrix4f toProjection = render.camera.GetPerspectiveMatrix(render.GetRenderSize());
	const Transform::Matrix toCamera = render.camera.transform.ToLocalMatrix();
	const Transform::Matrix toWorld = transform.ToWorldMatrix();
	Matrix4f cameraTransform = toProjection * (toCamera * toWorld).matrix();

	// Viewport transform
	v3 translate{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 0.f };
	v3 scale{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 100000000.f };
	Transform::Matrix toViewport = Eigen::Translation3f(translate) * Scaling(scale);


	for (i32 i = 0; i < vertices.Size(); ++i)
	{
		// Transform to camera perspective
		v4 vertex4;
		vertex4 << vertices[i], 1.f;
		vertex4 = cameraTransform * vertex4;

		const float divisor = 1.f / vertex4[3];

		// Transform to viewport
		v3_i32 v;
		v = (toViewport * (vertex4.head<3>() * divisor)).cast<i32>();
		outVertices[i] = v;
	}
}

void DrawMeshCommand::BackfaceCulling(const VertexBufferI32& vertices, TriangleBuffer& triangles)
{
	ZoneScoped("BackfaceCulling");

	// Remove all triangles that don't look towards the camera
	for (i32 i = 0; i < triangles.Size(); ++i)
	{
		const v3_u32& triangle = triangles[i];

		const v3_i32& v0 = vertices[triangle.x()];
		const v3_i32& v1 = vertices[triangle.y()];
		const v3_i32& v2 = vertices[triangle.z()];

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

void DrawMeshCommand::RenderTriangles(FrameRender& render, const TArray<v3_i32>& vertices, const TriangleBuffer& triangles)
{
	ZoneScoped("RasterizeTriangles");
	render.rasterizer.FillVertexBuffer(vertices, triangles, Color::Blue);
}
