// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <SDL.h>
#include <imgui/imgui.h>
#include <GL/gl3w.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>
#include <SDL_opengl.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

class Viewport {

	SDL_Window* window;
	SDL_GLContext gl_context;
	const char* glsl_version;

public:

	Viewport() {}

	bool Start() {
		// Setup SDL
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
			return false;

		if (!SetupRender())
		{
			if(window)
				SDL_DestroyWindow(window);
			SDL_Quit();
			return false;
		}

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		bool bFinish = false;
		while (!bFinish)
		{
			// Process window and input events
			SDL_PumpEvents();
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					bFinish = true;
				else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
					bFinish = true;
			}

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(window);
			ImGui::NewFrame();
			{
				// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
				if (show_demo_window)
					ImGui::ShowDemoWindow(&show_demo_window);

				// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
				{
					static float f = 0.0f;
					static int counter = 0;

					ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

					ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
					ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
					ImGui::Checkbox("Another Window", &show_another_window);

					ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
					ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

					if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
						counter++;
					ImGui::SameLine();
					ImGui::Text("counter = %d", counter);

					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					ImGui::End();
				}

				// 3. Show another simple window.
				if (show_another_window)
				{
					ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
					ImGui::Text("Hello from another window!");
					if (ImGui::Button("Close Me"))
						show_another_window = false;
					ImGui::End();
				}
			}

			// Rendering
			ImGui::Render();
			SDL_GL_MakeCurrent(window, gl_context);

			ImGuiIO& io = ImGui::GetIO();
			glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Present!
			SDL_GL_SwapWindow(window);
		}

		SDL_DestroyWindow(window);
		SDL_Quit();

		return true;
	}

private:

	bool SetupRender()
	{
#if PLATFORM_APPLE
		// GL 3.2 Core + GLSL 150
		glsl_version = "#version 150";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
		// GL 3.0 + GLSL 130
		glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		window = SDL_CreateWindow(
			"FecoEngine",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL
		);

		if (window == nullptr)
			return false;

		gl_context = SDL_GL_CreateContext(window);
		SDL_GL_SetSwapInterval(1); // Enable vsync

		// Initialize OpenGL loader
		if (gl3wInit() != 0)
			return false;

		//Setup UI
		if (!SetupUI())
			return false;

		// Create renderer
		return true;
	}

	bool SetupUI() {
		// Setup Dear ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Setup style
		ImGui::StyleColorsDark();

		return true;
	}
};
