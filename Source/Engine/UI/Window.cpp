// Copyright 2015-2019 Piperift - All rights reserved

#include "Window.h"


void Window::Build()
{
	bOpen = true;
	idName = TX("");
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
	// Generate unique name
	String name;
	if (!displayName.empty())
		name = CString::Printf("%s###%s", displayName.c_str(), idName.ToString().c_str());
	else
		name = idName.ToString();

	ImGui::Begin(name.c_str(), &bOpen, windowFlags);
}

void Window::EndWindow()
{
	ImGui::End();
}
