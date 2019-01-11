// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include <EASTL/queue.h>
#include <EASTL/shared_ptr.h>

#include "Core/Misc/DateTime.h"
#include "RenderCommand.h"
#include "TextureData.h"


struct Frame {
private:
	static u16 idCounter;
	u16 id;

public:

	DateTime time;

	TArray<eastl::shared_ptr<RenderCommand>> commands;


	Frame() : id(idCounter++), time(DateTime::Now()) {}
	Frame(const Frame& other) = delete;
	Frame& operator=(const Frame& other) = delete;

	void ScheduleCommand(eastl::shared_ptr<RenderCommand>&& command) {
		commands.Add(command);
	}

	void ExecuteCommands(Renderer& renderer) {
		for(const auto& command : commands)
		{
			command->Execute(renderer, *this);
		}
		commands.Empty();
	}

	u16 Id() { return id; }
};

struct FrameRender {
public:

	TextureData baseColor;


	FrameRender() = default;
};

