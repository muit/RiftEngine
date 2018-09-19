// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <SDL.h>
#include <imgui/imgui.h>
#include <GL/gl3w.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>
#include <SDL_opengl.h>

#include "EngineTypes.h"
#include "Object.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


enum class ERendererState : uint8 {
	Rendering,
	Failed
};

class Renderer : public Object {
	CLASS(Renderer, Object)

	ERendererState state;

	SDL_Window* window;
	SDL_GLContext gl_context;
	const char* glslVersion;


	bool showDemoWindow = true;
	bool showAnotherWindow = false;
	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:

	Renderer();

	void PrepareUI();

	virtual void BeforeDestroy() override;

	void PreTick();

	void TickUI(float deltaTime);

	void Render();

	ERendererState GetState() const { return state; }

	uint32 GetWindowId() const {
		return window ? SDL_GetWindowID(window) : 0;
	}
};
