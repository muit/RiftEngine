// � 2019 Miguel Fern�ndez Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Strings/String.h"


struct MaterialData
{
public:

	String vertexCode;
	String fragmentCode;


	MaterialData() = default;
	MaterialData(String vertexCode, String fragmentCode)
		: vertexCode{MoveTemp(vertexCode)}
		, fragmentCode{MoveTemp(fragmentCode)}
	{}

	bool Parse(const String& code);
};
