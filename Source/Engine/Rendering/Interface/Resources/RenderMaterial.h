// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include "Core/Strings/String.h"
#include "Core/Log.h"

#include "../../Data/MaterialData.h"
#include "Core/Strings/Name.h"
#include "Core/Math/Vector.h"

struct RenderMaterialParameter
{
	Name name;
	GLint id;
};

struct RenderMaterial
{
	GLint programId;

	TArray<RenderMaterialParameter> parameterIds;


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

	void Use() const { glUseProgram(programId); }

private:

	void CompileProgram(const String& vertexCode, const String& fragmentCode);

	void LogShaderError(GLint shaderId);
	void LogProgramError();

public:

	bool SetFloat(Name Id, float value) const;
	bool SetV3(Name Id, v3 value) const;
	bool SetMatrix4f(Name Id, Matrix4f value) const;
};
