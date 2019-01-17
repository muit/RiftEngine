// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Strings/String.h"
#include "Core/Log.h"


struct GLProgram {
	GLint programId;

	GLProgram() {}
	GLProgram(const String& vertexCode, const String& fragmentCode) {
		CompileProgram(vertexCode, fragmentCode);
		glUseProgram(programId);
	}

	void Use() {
		glUseProgram(programId);
	}

private:

	void CompileProgram(const String& vertexCode, const String& fragmentCode);

	void LogShaderError(GLint shaderId);
	void LogProgramError();
};
