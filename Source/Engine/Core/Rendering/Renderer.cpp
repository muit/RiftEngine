// Copyright 2015-2019 Piperift - All rights reserved

#include "Renderer.h"

#include <SDL_opengl.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>
#include <tracy/TracyOpenGL.hpp>

#include "Core/Log.h"
#include "World/World.h"


bool Renderer::Initialize()
{
#if PLATFORM_APPLE
	// GL 3.2 Core + GLSL 150
	glslVersion = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	glslVersion = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	window = SDL_CreateWindow(
		"Rift Engine",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
#if WITH_EDITOR
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
#else
		SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED
#endif
	);
	if (!window)
		return false;

	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); // Enable vsync


	//Setup GL and UI
	if (gl3wInit() != 0) {
		return false;
	}

	glRenderTexture = { SCREEN_WIDTH, SCREEN_HEIGHT };

	PrepareUI();
	TracyGpuContext(gl_context);
	return true;
}

void Renderer::PrepareUI()
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glslVersion);
}

void Renderer::PreTick()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
}

void Renderer::Render(Frame& frame)
{
	ZoneScopedNC("Render", 0x94d145);

	ImGui::Render();
	SDL_GL_MakeCurrent(window, gl_context);

	ImGuiIO& io = ImGui::GetIO();
	v2_u32 viewportSize{ (u32)io.DisplaySize.x, (u32)io.DisplaySize.y };

	glViewport(0, 0, viewportSize.x(), viewportSize.y());
	//glClearColor(0.7f, 0.4f, 0.4f, 1); No need to clear color, CPU rendering takes care of it
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// World Render
	{
		ZoneScopedNC("World", 0x94d145);

		// Clear texture to Red
		render.baseColor.Resize(viewportSize, Color::Red);

		//Log::Message("Commands: %i", frame.commands.Size());
		frame.ExecuteCommands(*this);

		// Render final base color into screen
		glRenderTexture.Draw(viewportSize, render.baseColor);
	}

	{ // UI Render
		ZoneScopedNC("UI", 0x94d145);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}

void Renderer::Sleep()
{
	ZoneScopedNC("Sleep", 0xD15545);
	SDL_GL_SwapWindow(window);
}

void Renderer::BeforeDestroy()
{
	Super::BeforeDestroy();

	if (window)
		SDL_DestroyWindow(window);

	//glDeleteTextures(1, &finalFrameId);
}

void Renderer::DrawImage(const v2_u32& position, const TextureData& texture)
{
	u32 width = texture.Size().x();
	u32 height = texture.Size().y();
	u32 pitch = render.baseColor.Size().x();
	u32 offset = pitch * position.y() + position.x();
	u32 delta = pitch - width;

	const Color* texturePixel = texture.Buffer().Data();
	Color* bcPixel = render.baseColor.Buffer().Data() + offset;

	while (height-- > 0)
	{
		for (Color* end = bcPixel + width; bcPixel < end; bcPixel++, texturePixel++)
		{
			*bcPixel = *texturePixel;
		}

		bcPixel += delta;
	}
}
