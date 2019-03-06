// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/queue.h>
#include <EASTL/shared_ptr.h>
#include <taskflow/taskflow.hpp>

#include "Core/Misc/DateTime.h"
#include "RenderCommand.h"
#include "Data/TextureData.h"
#include "Resources.h"
#include "Rasterizer.h"
#include "Data/LightData.h"


struct Frame {
private:
	static u16 idCounter;
	u16 id;

public:

	DateTime time;

	TArray<eastl::shared_ptr<RenderCommand>> commands;


	Frame() : id(idCounter++), time(DateTime::Now()) {}
	Frame(Frame&& other) = default;
	Frame& operator=(Frame&& other) = default;


	void ScheduleCommand(eastl::shared_ptr<RenderCommand>&& command) {
		commands.Add(command);
	}

	void ExecuteCommands(struct FrameRender& render);

	u16 Id() const { return id; }
};


// Where all render data is stored. Gets reused every frame.
struct FrameRender {
	TextureData baseColor;

	CameraData camera;

	LightingRender lighting;
	Resources resources;

	Rasterizer rasterizer;

	FrameRender()
		: baseColor{}
		, camera {}
		, lighting{}
		, resources{}
		, rasterizer{ baseColor }
	{}

	void NewFrame(v2_u32 viewportSize) {
		ZoneScopedNC("NewFrame", 0x94d145);
		//Clean screen?
		baseColor.Clear(viewportSize); /* Yes  +- 6ms */
		/*baseColor.Resize(viewportSize); No          */

		rasterizer.Clear();

		lighting.directionals.Empty();
		lighting.points.Empty();
	}

	void DrawImage(const v2_u32& position, const TextureData& texture);

	v2_u32 GetRenderSize() const { return baseColor.Size(); }


	FORCEINLINE CameraData& Camera() { return camera; }
	FORCEINLINE const CameraData& Camera() const { return camera; }
};

