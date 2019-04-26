// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "EngineTypes.h"
#include <SDL.h>
#include <imgui/imgui.h>
#include <EASTL/queue.h>

#include "Frame.h"
#include "Resources.h"
#include "Commands/RenderCommand.h"
#include "Interface/ScreenTexture.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


class Renderer : public Object
{
	CLASS(Renderer, Object)


	static const char* glslVersion;

	SDL_Window* window;
	SDL_GLContext gl_context;

	ScreenTexture glRenderTexture;

	TArray<Frame> frameCache{2};
	u32 gameFrameId = 0;

	FrameRender render;


public:

	Renderer();

	bool Initialize();

	void PrepareUI();

	void PreTick();

	void Render();

	void SwapWindow();

	virtual void BeforeDestroy() override;


	u32 GetWindowId() const { return window ? SDL_GetWindowID(window) : 0; }


	/** Frame being prepared on game thread */
	Frame& GetGameFrame() { return frameCache[gameFrameId]; }
	/** Frame being rendered on graphics thread */
	Frame& GetRenderFrame() { return frameCache[(gameFrameId + 1) % 2]; }

	void SwitchFrameBuffer() { gameFrameId = (gameFrameId + 1) % 2; }

	template<typename Command, typename ...Args>
	void QueueCommand(Args... args)
	{
		static_assert(eastl::is_base_of<RenderCommand, Command>::value, "Command type must inherit RenderCommand");

		GetGameFrame().ScheduleCommand(
			eastl::make_shared<Command>(eastl::forward<Args>(args)...)
		);
	}
};
