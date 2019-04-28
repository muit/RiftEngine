// © 2019 Miguel Fernández Arce - All rights reserved

#include "MeshCommands.h"

#include "Core/Engine.h"
#include "Core/Math/Vector.h"
#include "Tools/Profiler.h"
#include "tracy/TracyOpenGL.hpp"


using MeshTransformMap = eastl::unordered_map<Name, TArray<Transform>>;
using MaterialBatchMap = eastl::unordered_map<Name, MeshTransformMap>;


void DrawMeshesCommand::Execute(FrameRender& render, Frame& frame)
{
	ScopedGraphicsZone("Draw Mesh Command");
	TracyGpuZone("Draw Mesh Command");

	MaterialBatchMap batches {};

	/** Prepare batches
	 * Group them by Material -> Mesh -> Transform
	 */
	{
		ScopedGraphicsZone("Build Batches");
		for (const auto& meshInstance : meshes)
		{
			// Find or add a Material Batch
			auto it = batches.find(meshInstance.material.GetPath());
			if (it != batches.end())
			{
				auto& meshBatches = it->second;

				// Find or add a Mesh Batch
				auto meshIt = meshBatches.find(meshInstance.mesh.GetPath());
				if (meshIt != meshBatches.end())
				{
					// Found batch, add an instance
					meshIt->second.Add(meshInstance.transform);
				}
				else
				{
					// New Mesh batch with this meshInstance
					meshBatches.insert({
						meshInstance.mesh.GetPath(),
						TArray<Transform>{ meshInstance.transform }
					});
				}
			}
			else
			{
				// New Material batch with this meshInstance
				batches.insert({
					meshInstance.material.GetPath(),
					MeshTransformMap {
						{
							meshInstance.mesh.GetPath(),
							TArray<Transform>{ meshInstance.transform }
						}
					}
				});
			}
		}
	}

	/** Draw batches */
	static const Name transformParameter = { "u_mvp" };

	// Viewport transform
	const v3 translate{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 0.f };
	const v3 scale{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 100000000.f };

	const Transform::Matrix toViewport = Eigen::Translation3f(translate) * Scaling(scale);
	const Matrix4f toProjection{ render.Camera().GetPerspectiveMatrix(render.GetRenderSize()) };
	const Matrix4f toCamera{ render.Camera().transform.ToLocalMatrix().matrix() };
	const Matrix4f cameraTransform{ toViewport * toProjection * toCamera };

	for(const auto& materialBatch : batches)
	{
		ScopedGraphicsZone("Material Batch");

		const RenderMaterial& materialResource = render.resources.Get<ResourceType::Material>(materialBatch.first);

		materialResource.Use();

		for (const auto& meshBatch : materialBatch.second)
		{
			ScopedGraphicsZone("Mesh Batch");

			const RenderMesh& meshResource = render.resources.Get<ResourceType::Mesh>(meshBatch.first);

			// Bind mesh once
			meshResource.Bind();

			for (const auto& transform : meshBatch.second)
			{
				ScopedGraphicsZone("Transform Mesh Draw");

				// Update transform on material and draw triangles
				materialResource.SetMatrix4f(transformParameter, cameraTransform * transform.ToWorldMatrix().matrix());
				meshResource.Draw();
			}
		}
	}
	RenderMesh::Unbind();

	/**
	// #TODO: Cache operation buffers on resources
	TriangleBuffer triangles{ mesh.GetTriangles() };
	NormalsBuffer normals = mesh.GetNormals();

	// Reserve same size
	u32 verticesCount = (u32)mesh.GetVertices().Size();

	// Vertex shader params
	VertexBuffer vertices{ mesh.GetVertices() };
	LColorBuffer  colors{ verticesCount, { color } };

	VertexBufferI32 screenVertices{ verticesCount };


	TaskFlow flow = GEngine->Tasks().CreateFlow();

	Task wVertexTask         = flow.emplace(VertexToWorld(vertices));
	Task wNormalTask         = flow.emplace(NormalToWorld(normals));
	Task sVertexTask         = flow.emplace(TransformToScreen(render, vertices, screenVertices));
	Task backfaceCullingTask = flow.emplace(BackfaceCulling(screenVertices, triangles));
	Task vertexShaderTask    = flow.emplace(OperateVertexShader(render, vertices, normals, colors));

	// Build async graph
	{
		wVertexTask.precede(sVertexTask);
		wVertexTask.precede(vertexShaderTask);
		wNormalTask.precede(vertexShaderTask);

		sVertexTask.precede(backfaceCullingTask);
	}

	flow.wait_for_all();

	RenderTriangles(render, screenVertices, triangles, colors);*/
}

/*SubTaskLambda DrawMeshCommand::VertexToWorld(VertexBuffer& vertices)
{
	const Transform t = transform;

	return [&vertices, t](tf::SubflowBuilder& sbf)
	{
		ZoneScopedN("Vertices to world: Splitter");

		const i32 size = vertices.Size();
		const i32 halfSize = size / 3;

		// Split execution between 3 threads
		sbf.parallel_for(0, size, halfSize,
		[&vertices, t, halfSize](const i32 startIndex)
		{
			ZoneScopedN("Vertices to world");
			const Transform::Matrix toWorld = t.ToWorldMatrix();

			const i32 endIndex = Math::Min(startIndex + halfSize, vertices.Size());

			for (i32 i = startIndex; i < endIndex; ++i)
			{
				// Vertices to world
				v3& vertex = vertices[i];
				vertex = toWorld * vertex;
			}
		});
	};
}


SubTaskLambda DrawMeshCommand::NormalToWorld(NormalsBuffer& normals)
{
	const Transform t = transform;

	return [&normals, t](tf::SubflowBuilder& sbf)
	{
		ZoneScopedN("Normals to world: Splitter");

		const i32 size = normals.Size();
		const i32 halfSize = size / 3;

		// Split execution between 3 threads
		sbf.parallel_for(0, size, halfSize,
		[&normals, t, halfSize](const i32 startIndex)
		{
			ZoneScopedN("Normals to world");
			const auto toWorldLinear = t.ToWorldMatrix().linear();

			const i32 endIndex = Math::Min(startIndex + halfSize, normals.Size());

			for (i32 i = startIndex; i < endIndex; ++i)
			{
				// Normals to world
				v3& normal = normals[i];
				normal = toWorldLinear * normal;
			}
		});
	};
}

TaskLambda DrawMeshCommand::OperateVertexShader(FrameRender& render, const VertexBuffer& worldVertices, const NormalsBuffer& normals, LColorBuffer& colors)
{
	return [&render, &worldVertices, &normals, &colors]()
	{
		ZoneScopedN("Vertex Shader");

		AmbientLightData ambientLight{ Color::White, 0.8f };

		// Cache all directional light directions
		TArray<v3> directionalForwards;
		directionalForwards.Reserve(render.lighting.directionals.Size());

		for (i32 i = 0; i < render.lighting.directionals.Size(); ++i)
		{
			v3 dirForward = render.lighting.directionals[i].rotation.GetForward();
			dirForward.normalize();
			directionalForwards.Add(dirForward);
		}

		// For each vertex
		LinearColor lightColor;
		for (i32 i = 0; i < worldVertices.Size(); ++i)
		{
			const v3& normal = normals[i];

			// Calculate ambient light
			lightColor = ambientLight.color * ambientLight.intensity;

			// Apply directional lights
			for (i32 j = 0; j < render.lighting.directionals.Size(); ++j)
			{
				const DirectionalLightData& directional = render.lighting.directionals[j];
				const v3& forward = directionalForwards[j];

				const float NoL = forward.dot(normal);
				lightColor += (directional.color * -NoL * directional.intensity);
			}

			// Apply point lights
			/*{
				const v3& vertex = worldVertices[i];

				const float NoL = (vertex - point.location).dot(normals[i]);
				color += point.color * -NoL * point.intensity;
			}*//*

			// Apply light to vertex color
			colors[i] *= lightColor;
		}
	};
}

SubTaskLambda DrawMeshCommand::TransformToScreen(const FrameRender& render, const VertexBuffer& worldVertices, VertexBufferI32& screenVertices)
{
	return [&render, &worldVertices, &screenVertices](tf::SubflowBuilder& sbf)
	{
		ZoneScopedN("Transform To Camera: Splitter");

		// Viewport transform
		const v3 translate{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 0.f };
		const v3 scale{ float(render.GetRenderSize().x() / 2), float(render.GetRenderSize().y() / 2), 100000000.f };

		const Transform::Matrix toViewport = Eigen::Translation3f(translate) * Scaling(scale);
		const Matrix4f toProjection{ render.Camera().GetPerspectiveMatrix(render.GetRenderSize()) };
		const Matrix4f toCamera    { render.Camera().transform.ToLocalMatrix().matrix()           };

		const Matrix4f cameraTransform{ toViewport * toProjection * toCamera };


		const i32 size = worldVertices.Size();
		const i32 halfSize = size / 3;

		// Split execution between 3 threads
		sbf.parallel_for(0, size, halfSize,
		[cameraTransform, &worldVertices, &screenVertices, halfSize](const i32 startIndex)
		{
			ZoneScopedN("Transform To Camera");

			const i32 endIndex = Math::Min(startIndex + halfSize, worldVertices.Size());

			v4 vertex4;
			for (i32 i = startIndex; i < endIndex; ++i)
			{
				// Transform: camera -> projection -> viewport
				vertex4 << worldVertices[i], 1.f;
				vertex4 = cameraTransform * vertex4;

				screenVertices[i] = (vertex4.head<3>() / vertex4.w()).cast<i32>();
			}
		});
	};
}

TaskLambda DrawMeshCommand::BackfaceCulling(const VertexBufferI32& vertices, TriangleBuffer& triangles)
{
	return [&vertices, &triangles]() {
		ZoneScopedN("Backface Culling");

		// Remove all triangles that don't look towards the camera
		for (i32 i = 0; i < triangles.Size(); ++i)
		{
			const v3_u32& triangle = triangles[i];

			const v3_i32& v0 = vertices[triangle.x()];
			const v3_i32& v1 = vertices[triangle.y()];
			const v3_i32& v2 = vertices[triangle.z()];

			const bool lookingFront = ((v1.x() - v0.x()) * (v2.y() - v0.y()) - (v2.x() - v0.x()) * (v1.y() - v0.y()) > 0.f);

			if (lookingFront)
			{
				triangles.RemoveAtSwap(i, false);
				--i;
			}
		}
		triangles.Shrink();
	};
}

void DrawMeshCommand::RenderTriangles(FrameRender& render, const TArray<v3_i32>& vertices, const TriangleBuffer& triangles, const LColorBuffer& colors)
{
	ZoneScopedN("Rasterize Triangles");
	render.rasterizer.FillVertexBuffer(vertices, triangles, colors);
}*/
