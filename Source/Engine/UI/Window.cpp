// Copyright 2015-2019 Piperift - All rights reserved

#include "Window.h"


void Window::Build()
{
	bOpen = true;
	windowName = "";
	windowFlags = ImGuiWindowFlags_None;
}

void Window::Tick()
{
	BeginWindow();
	EndWindow();
}

void Window::BeginWindow()
{
	ImGui::Begin(windowName.ToString().c_str(), &bOpen, windowFlags);
}

void Window::EndWindow()
{
	ImGui::End();
}
