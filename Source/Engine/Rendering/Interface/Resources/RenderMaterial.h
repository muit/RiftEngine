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
	static const Name notAResourceId;

	GLuint programId = GL_INVALID_INDEX;

	TArray<RenderMaterialParameter> parameterIds;


	RenderMaterial() = default;
	RenderMaterial(const String& vertexCode, const String& fragmentCode, Name id = notAResourceId)
	{
		CompileProgram(id, vertexCode, fragmentCode);
	}
	RenderMaterial(Name id, const MaterialData& materialData)
		: RenderMaterial{ materialData.vertexCode, materialData.fragmentCode, id }
	{}

	RenderMaterial(RenderMaterial&& other) : programId{other.programId} {
		other.programId = GL_INVALID_INDEX;
	}
	RenderMaterial& operator=(RenderMaterial&& other) {
		programId = other.programId;
		other.programId = GL_INVALID_INDEX;
		return *this;
	}

	~RenderMaterial() {
		glDeleteProgram(programId);
	}

	void Use() const { glUseProgram(programId); }
	void BindTextures() const {}

private:

	void CompileProgram(Name id, const String& vertexCode, const String& fragmentCode);

	void LogShaderError(GLint shaderId);
	void LogProgramError();

public:

	bool SetFloat(Name Id, float value) const;
	bool SetV3(Name Id, const v3& value) const;
	bool SetMatrix4f(Name Id, const Matrix4f& value) const;
};
