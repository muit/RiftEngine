// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "Widget.h"
#include <imgui/imgui.h>


class Window : public Widget {
	CLASS(Window, Widget)

protected:

	bool bOpen;
	Name windowName;
	ImGuiWindowFlags windowFlags;


	virtual void Build() override;
	virtual void Tick() override;

	void BeginWindow();
	void EndWindow();

public:

	void SetOpen(bool bValue) { bOpen = bValue; }
	bool IsOpened() const { return bOpen; }
	bool* IsOpenedPtr() { return &bOpen; }
};