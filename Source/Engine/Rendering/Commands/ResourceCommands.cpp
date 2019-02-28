// © 2019 Miguel Fernández Arce - All rights reserved

#include "ResourceCommands.h"
#include "Core/Math/Vector.h"


void DrawMeshCommand::Execute(FrameRender& render, Frame& frame)
{
	const MeshData& mesh = render.resources.Get<ResourceType::Mesh>(id);

	TriangleBuffer triangles{ mesh.GetTriangles() };
	NormalsBuffer normals = mesh.GetNormals();

	// Reserve same size
	u32 verticesCount = (u32)mesh.GetVertices().Size();

	// Vertex shader params
	VertexBuffer vertices{ mesh.GetVertices() };
	LColorBuffer  colors { verticesCount, LinearColor::Green};

	VertexBufferI32 screenVertices{ verticesCount };

	TransformToWorld(render, vertices, normals);

	OperateVertexShader(render, vertices, normals, colors);

	TransformToScreen(render, vertices, screenVertices);

	BackfaceCulling(screenVertices, triangles);
	RenderTriangles(render, screenVertices, triangles, colors);
}

void DrawMeshCommand::TransformToWorld(FrameRender& render, VertexBuffer& vertices, NormalsBuffer& normals)
{
	ZoneScoped("TransformToWorld");

	const Transform::Matrix toWorld = transform.ToWorldMatrix();
	const auto toWorldLinear = toWorld.linear();

	for (i32 i = 0; i < vertices.Size(); ++i)
	{
		// Vertices to world
		auto& vertex = vertices[i];
		vertex = toWorld * vertex;

		// Normals to world
		v3& normal = normals[i];
		normal = toWorldLinear * normal;
	}
}

void DrawMeshCommand::OperateVertexShader(FrameRender& render, const VertexBuffer& worldVertices, const NormalsBuffer& normals, LColorBuffer& colors)
{
	// #TODO: We should stack all directional lights here into one

	if (render.lighting.directionals.Size() > 0)
	{
		// Copy to make it local (therefore faster)
		DirectionalLightData directional = render.lighting.directionals[0];
		v3 dirForward = directional.rotation.GetForward();
		dirForward.normalize();

		for (i32 i = 0; i < worldVertices.Size(); ++i)
		{
			const v3& normal = normals[i];

			LinearColor& color = colors[i];

			// Apply directional
			const float NoL = dirForward.dot(normal);
			color *= (directional.color * -NoL * directional.intensity);

			// Apply points
			/*{
				const v3& vertex = worldVertices[i];

				const float NoL = (vertex - point.location).dot(normals[i]);
				color += point.color * -NoL * point.intensity;
			}*/
		}
	}
}

void DrawMeshCommand::TransformToScreen(FrameRender& render, const VertexBuffer& worldVertices, VertexBufferI32& screenVertices)
{
	ZoneScoped("TransformToCamera");

	const Matrix4f toProjection = render.camera.GetPerspectiveMatrix(render.GetRenderSize());
	const Transform::Matrix toCamera = render.camera.transform.ToLocalMatrix();
	Matrix4f cameraTransform = toProjection * (toCamera).matrix();

	// Viewport transform
	v3 translate{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 0.f };
	v3 scale{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 100000000.f };
	Transform::Matrix toViewport = Eigen::Translation3f(translate) * Scaling(scale);

	for (i32 i = 0; i < worldVertices.Size(); ++i)
	{
		// Transform to camera perspective
		v4 vertex4;
		vertex4 << worldVertices[i], 1.f;
		vertex4 = cameraTransform * vertex4;

		const float divisor = 1.f / vertex4[3];

		// Transform to viewport
		screenVertices[i] = (toViewport * (vertex4.head<3>() * divisor)).cast<i32>();
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

void DrawMeshCommand::RenderTriangles(FrameRender& render, const TArray<v3_i32>& vertices, const TriangleBuffer& triangles, const LColorBuffer& colors)
{
	ZoneScoped("RasterizeTriangles");
	render.rasterizer.FillVertexBuffer(vertices, triangles, colors);
}
