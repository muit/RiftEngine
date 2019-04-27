// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Strings/String.h"
#include "Core/Log.h"

#include "../../Data/MaterialData.h"


struct RenderMaterial
{

	GLint programId;


	RenderMaterial() : programId{ 0 } {}
	RenderMaterial(const String& vertexCode, const String& fragmentCode)
	{
		CompileProgram(vertexCode, fragmentCode);
		glUseProgram(programId);
	}
	RenderMaterial(const MaterialData& materialData)
		: RenderMaterial{ materialData.vertexCode, materialData.fragmentCode }
	{}

	RenderMaterial(RenderMaterial&& other) : programId{other.programId} {
		other.programId = 0;
	}
	RenderMaterial& operator=(RenderMaterial&& other) {
		programId = other.programId;
		other.programId = 0;
		return *this;
	}

	~RenderMaterial() {
		glDeleteProgram(programId);
	}

	void Use() { glUseProgram(programId); }

private:

	void CompileProgram(const String& vertexCode, const String& fragmentCode);

	void LogShaderError(GLint shaderId);
	void LogProgramError();
};
