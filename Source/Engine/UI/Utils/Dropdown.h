// Copyright 2015-2019 Piperift - All rights reserved

#include <imgui/imgui.h>
#include "Core/Math/Vector.h"


namespace ImGui
{
	bool BeginButtonDropDown(const char* label, v2 buttonSize)
	{
		ImGui::SameLine(0.f, 0.f);

		ImGuiContext* context = ImGui::GetCurrentContext();
		const ImGuiStyle& style = context->Style;

		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		ImVec2 size(20, buttonSize.y);
		bool pressed = ImGui::Button("##", size);

		// Arrow
		v2 center(context->Pos.x + x + 10, context->Pos.y + y + buttonSize.y / 2);
		float r = 8.f;
		center.y -= r * 0.25f;
		v2 a = center + v2{ 0.f, 1.f } * r;
		v2 b = center + v2{ -0.866f, -0.5f } * r;
		v2 c = center + v2{ 0.866f, -0.5f } * r;

		context->CurrentWindow->DrawList->AddTriangleFilled(a, b, c, GetColorU32(ImGuiCol_Text));

		// Popup

		ImVec2 popupPos;

		popupPos.x = context->Pos.x + x - buttonSize.x;
		popupPos.y = context->Pos.y + y + buttonSize.y;

		ImGui::SetNextWindowPos(popupPos);

		if (pressed)
		{
			ImGui::OpenPopup(label);
		}

		if (ImGui::BeginPopup(label))
		{
			ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, style.Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, style.Colors[ImGuiCol_Button]);
			return true;
		}

		return false;
	}

	void EndButtonDropDown()
	{
		ImGui::PopStyleColor(3);
		ImGui::EndPopup();
	}
}