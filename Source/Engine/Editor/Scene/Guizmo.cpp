// Copyright 2015-2019 Piperift - All rights reserved

#include "Guizmo.h"

#if WITH_EDITOR
#include <ImGui/imgui.h>
#include <ImGui/ImGuizmo.h>

#include "World.h"
#include "Core/Reflection/Runtime/HandleHelper.h"
#include "Core/Reflection/ReflectionTags.h"
#include "Core/Object/Object.h"
#include "ECS/ECSManager.h"
#include "ECS/Component.h"

#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Singletons/CActiveCamera.h"


void Guizmo::Tick(float deltaTime)
{
	auto ecs = GetWorld()->GetECS();
	assert(ecs.IsValid());

	if (!ecs->IsValid(entity) || !ecs->Has<CTransform>(entity))
	{
		return;
	}

	if (auto* cameraComp = ecs->FindSingleton<CActiveCamera>())
	{
		CTransform& transform = ecs->Get<CTransform>(entity);

		ImGuiViewport* vp = ImGui::GetMainViewport();
		Matrix4f projection = cameraComp->activeData.GetProjectionMatrix({ vp->Size.x, vp->Size.y });
		Matrix4f view = cameraComp->activeData.GetViewMatrix();

		ImGuizmo::SetRect(vp->Pos.x, vp->Pos.y, vp->Size.x, vp->Size.y);

		// Draw Grid
		{
			static Matrix4f identity = Matrix4f::Identity();
			ImGuizmo::DrawGrid(&view[0].x, &projection[0].x, &identity[0].x, 1.f);
		}

		// Draw Axis
		{
			Matrix4f matrix = transform.transform.ToMatrix();
			ImGuizmo::DrawCube(&view[0].x, &projection[0].x, &matrix[0].x);
			ImGuizmo::Manipulate(&view[0].x, &projection[0].x, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, &matrix[0].x, nullptr, nullptr);
			transform.transform.SetFromMatrix(matrix);
		}
	}
}

void Guizmo::SetEntity(EntityId inEntity)
{
	if (inEntity != entity)
	{
		entity = inEntity;
		ForceRebuild();
	}
}

#endif
