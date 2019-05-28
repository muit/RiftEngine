// © 2019 Miguel Fernández Arce - All rights reserved

#include "MeshCommands.h"

#include "Core/Engine.h"
#include "Core/Math/Vector.h"
#include "Tools/Profiler.h"
#include "tracy/TracyOpenGL.hpp"


using MeshTransformMap = TMap<Name, TArray<Transform>>;
using MaterialBatchMap = TMap<Name, MeshTransformMap>;


void DrawMeshesCommand::Execute(FrameRender& render, Frame& frame)
{
	ScopedGraphicsZone("Draw Mesh Command");

	static const Name mvpParameter { "mvp" };
	static const Name modelParameter { "model" };

	const Matrix4f view{ render.Camera().GetViewMatrix() };
	const Matrix4f projection{ render.Camera().GetProjectionMatrix(render.GetRenderSize()) };
	const Matrix4f vpMatrix{ projection * view };

	/** Prepare batches
	 * Group them by Material -> Mesh -> Transform
	 */
	MaterialBatchMap batches{ Name::None() };
	{
		ScopedGraphicsZone("Build Batches");
		for (const auto& meshInstance : meshes)
		{
			// Find or add a Material Batch
			if (auto* meshBatches = batches.Find(meshInstance.material.GetPath()))
			{
				// Find or add a Mesh Batch
				if (auto* mesh = meshBatches->Find(meshInstance.mesh.GetPath()))
				{
					// Found batch, add an instance
					mesh->Add(meshInstance.transform);
				}
				else
				{
					// New Mesh batch with this meshInstance
					meshBatches->Insert(
						meshInstance.mesh.GetPath(),
						TArray<Transform>{ meshInstance.transform }
					);
				}
			}
			else
			{
				MeshTransformMap newMeshBatches {
					{
						meshInstance.mesh.GetPath(),
						TArray<Transform>{ meshInstance.transform }
					},
					Name::None()
				};

				// New Material batch with this meshInstance
				batches.Insert(
					meshInstance.material.GetPath(),
					MoveTemp(newMeshBatches)
				);
			}
		}
	}

	/** Draw batches */
	for (const auto& materialBatch : batches)
	{
		ScopedGraphicsZone("Material Batch");

		const RenderMaterial& material = render.resources.Get<ResourceType::Material>(materialBatch.first);
		material.Use();

		for (const auto& meshBatch : materialBatch.second)
		{
			ScopedGraphicsZone("Mesh Batch");

			const RenderMesh& meshResource = render.resources.Get<ResourceType::Mesh>(meshBatch.first);

			// Bind mesh once
			meshResource.Bind();

			material.BindStaticParameters(render.resources);
			material.SetV3("camera_position", render.camera.transform.location);
			render.lighting.Bind(material);

			for (const auto& transform : meshBatch.second)
			{
				ScopedGraphicsZone("Transform Mesh Draw");

				// Update transform on material and draw triangles
				const Matrix4f model = transform.ToMatrix();
				const Matrix4f mvp = vpMatrix * model;
				material.SetMatrix4f(mvpParameter, mvp);
				material.SetMatrix4f(modelParameter, model);

				meshResource.Draw();
			}
		}
	}
	RenderMesh::Unbind();
}
