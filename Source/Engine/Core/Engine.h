// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Rendering/Renderer.h"

#include "Core/World.h"
#include "Input/InputManager.h"
#include "UI/UIManager.h"
#include "Rendering/Renderer.h"
#include "Misc/Time.h"
#include "MultiThreading.h"


class Engine : public Object {
	CLASS(Engine, Object)

	FrameTime frameTime;

	TaskSystem taskSystem;

	GlobalPtr<World> world;
	GlobalPtr<InputManager> input;
	GlobalPtr<UIManager> ui;
	GlobalPtr<Renderer> renderer;

public:

	Engine() : Super() {}


	/** Begin Object interface */
protected:
	virtual void BeforeDestroy() override { Shutdown(); }
public:
	virtual Ptr<World> GetWorld() const override { return world; }
	/** End Object interface */

private:

	bool Start();

	void Loop(bool& bFinish);

	void Shutdown() {
		SDL_Quit();
	}


public:

	Ptr<InputManager> Input() { return input; }
	Ptr<Renderer> GetRenderer() { return renderer; }

	TaskSystem& Tasks() { return taskSystem; }

	/** STATIC */

	static void StartEngine()
	{
		GlobalPtr<Engine> engine{ Create<Engine>() };
		globalEngine = engine.AsPtr();

		engine->Start();
	}

	static Ptr<Engine> GetEngine() { return globalEngine; }

private:

	static Ptr<Engine> globalEngine;
};

#define GEngine Engine::GetEngine()


template<typename Command, typename ...Args>
FORCEINLINE void QueueRenderCommand(Args... args)
{
	GEngine->GetRenderer()->QueueCommand<Command>(eastl::forward<Args>(args)...);
}