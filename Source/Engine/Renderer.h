// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"
#include "EngineTypes.h"
#include <SDL.h>
#include <imgui/imgui.h>

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


public:

	Renderer();

	void PrepareUI();

	virtual void BeforeDestroy() override;

	void PreTick();

	void Render();

	ERendererState GetState() const { return state; }

	SDL_Window* GetWindow() { return window; }

	uint32 GetWindowId() const {
		return window ? SDL_GetWindowID(window) : 0;
	}
};
