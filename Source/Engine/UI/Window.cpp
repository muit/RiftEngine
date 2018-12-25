// Copyright 2015-2019 Piperift - All rights reserved

#include "Window.h"


void Window::Build()
{
	bOpen = true;
	windowName = TX("");
	windowFlags = 0;
}

void Window::Tick()
{
	if (bOpen)
	{
		BeginWindow();
		TickChilds();
		EndWindow();
	}
}

void Window::BeginWindow()
{
	ImGui::Begin(windowName.ToString().c_str(), &bOpen, windowFlags);
}

void Window::EndWindow()
{
	ImGui::End();
}
