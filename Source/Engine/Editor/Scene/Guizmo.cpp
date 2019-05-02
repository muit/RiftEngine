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

#include "Gameplay/Singletons/CActiveCamera.h"


void Guizmo::Tick(float deltaTime)
{
	auto ecs = GetWorld()->GetECS();
	assert(ecs.IsValid());

	if (!ecs->IsValid(entity))
	{
		return;
	}

	if (auto * cameraComp = ecs->FindSingleton<CActiveCamera>())
	{
		ImGuiViewport* vp = ImGui::GetMainViewport();

		Matrix4f matrix = testTransform.ToMatrix().Transpose();
		Matrix4f projection = cameraComp->activeData.GetProjectionMatrix({ vp->Size.x, vp->Size.y });
		Matrix4f view = cameraComp->activeData.GetViewMatrix();
		static Matrix4f identity = Matrix4f::Identity();

		v3 pos = testTransform.location;
		v3 camPos = cameraComp->activeData.transform.location;
		Log::Message("Cam(x:%.2f y:%.2f z:%.2f) - Pos(x:%.2f y:%.2f z:%.2f)", camPos.x, camPos.y, camPos.z, pos.x, pos.y, pos.z);

		ImGuizmo::SetRect(vp->Pos.x, vp->Pos.y, vp->Size.x, vp->Size.y);

		ImGuizmo::DrawCube(&view[0].x, &projection[0].x, &identity[0].x);
		ImGuizmo::DrawGrid(&view[0].x, &projection[0].x, &identity[0].x, 1.f);

		ImGuizmo::Manipulate(&view[0].x, &projection[0].x, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, &identity[0].x, nullptr, nullptr);

		testTransform.SetFromMatrix(matrix);
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
