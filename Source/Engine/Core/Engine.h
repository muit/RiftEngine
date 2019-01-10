// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Rendering/Renderer.h"
#include <tracy/Tracy.hpp>

#include "World/World.h"
#include "UI/UIManager.h"
#include "Misc/Time.h"


class Engine : public Object {
	CLASS(Engine, Object)

	FrameTime frameTime;
	GlobalPtr<World> world;
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

	bool Start() {
		{
			ZoneScopedN("Start-Up");

			// Setup SDL
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
				return false;

			renderer = Create<Renderer>();
			if (!renderer->Initialize())
				return false;

			world = Create<World>(GetSelf());
			world->Start();

			ui = Create<UIManager>(GetSelf());
			ui->Prepare();
		}

		frameTime = {};
		bool bFinish = false;
		while (!bFinish)
		{
			ZoneScoped("Tick");
			TracyMessageL("Tick");
			Loop(bFinish);
		}

		return true;
	}

	void Loop(bool& bFinish) {
		// New frame
		Frame frame = {};

		{
			String tickName = CString::Printf(TX("Tick %i"), frame.Id()).c_str();
			FrameMarkStart(tickName.c_str());

			frameTime.Tick();

			// Process window and input events
			SDL_PumpEvents();
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ui->OnSDLEvent(&event);
				if (event.type == SDL_QUIT)
					bFinish = true;
				else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == renderer->GetWindowId())
					bFinish = true;
			}

			renderer->PreTick();

			world->Tick(frameTime.deltaTime);
			ui->Tick(frameTime.deltaTime);

			// Rendering
			renderer->Render();

			FrameMarkEnd(tickName.c_str());
		}
		renderer->Sleep();
	}

	void Shutdown() {
		SDL_Quit();
	}


	/** STATIC */
public:

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
