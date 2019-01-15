// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "EngineTypes.h"
#include <SDL.h>
#include <imgui/imgui.h>
#include <EASTL/queue.h>

#include "Frame.h"
#include "Resources.h"
#include "RenderCommand.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


class Renderer : public Object {
	CLASS(Renderer, Object)


	SDL_Window* window;
	SDL_GLContext gl_context;
	const char* glslVersion;

	GLuint finalFrameId;
	FrameRender render;

public:
	// Render thread only
	Resources resources;


public:

	Renderer() : Super() {}

	bool Initialize();

	void PrepareUI();

	void PreTick();

	void Render(Frame& frame);

	void Sleep();

	virtual void BeforeDestroy() override;


	u32 GetWindowId() const {
		return window ? SDL_GetWindowID(window) : 0;
	}


	void DrawImage(Frame& frame, const v2_u32& position, const TextureData& texture);
};
