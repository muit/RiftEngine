// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreEngine.h"
#include <EASTL/unordered_map.h>
#include "Core/Strings/String.h"
#include "Core/Log.h"

#include "../../Data/MaterialData.h"
#include "Core/Strings/Name.h"
#include "Core/Math/Vector.h"


struct RenderMaterial
{
	static const Name notAResourceId;

	GLuint programId = GL_INVALID_INDEX;

	TArray<MaterialTextureParam> textures;
	mutable TMap<Name, GLint> parameterIds{ Name::None() };


	RenderMaterial() = default;

	RenderMaterial(Name id, const MaterialData& data)
	{
		CompileProgram(id, data.vertexCode, data.fragmentCode);
		textures = data.textureParams;
	}

	RenderMaterial(RenderMaterial&& other) : programId{ other.programId }, textures{ other.textures } {
		other.programId = GL_INVALID_INDEX;
	}
	RenderMaterial& operator=(RenderMaterial&& other) {
		programId = other.programId;
		textures = other.textures;
		other.programId = GL_INVALID_INDEX;
		return *this;
	}

	~RenderMaterial() {
		glDeleteProgram(programId);
	}

	void Use() const { glUseProgram(programId); }
	void BindStaticParameters(const struct Resources& resources) const;


	bool SetFloat(Name name, float value) const;
	bool SetI32(Name name, i32 value) const;
	bool SetV3(Name name, const v3& value) const;
	bool SetMatrix4f(Name name, const Matrix4f& value) const;

private:

	void CompileProgram(Name id, const String& vertexCode, const String& fragmentCode);

	void LogShaderError(GLint shaderId);
	void LogProgramError();

	GLint FindParameterIndex(const Name& name) const;
};
