// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/queue.h>
#include <EASTL/shared_ptr.h>

#include "Core/Misc/DateTime.h"
#include "RenderCommand.h"
#include "TextureData.h"
#include "Resources.h"


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

	Resources resources;

	Transform camera;


	FrameRender() = default;

	void DrawImage(const v2_u32& position, const TextureData& texture);
};

