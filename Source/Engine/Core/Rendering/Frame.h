// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"

#include "TextureData.h"
#include "../Misc/DateTime.h"


struct Frame {

	DateTime time;
	v2_u32 size;
	TextureData baseColor;

	Frame(v2_u32 size) : time(DateTime::Now()), size{size}, baseColor{ size } {}
};
