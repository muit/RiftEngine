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
	mutable TMap<Name, GLint> parameterIds{ Name::None() };


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


	bool SetFloat(Name name, float value) const;
	bool SetI32(Name name, i32 value) const;
	bool SetV3(Name name, const v3& value) const;
	bool SetMatrix4f(Name name, const Matrix4f& value) const;

private:

	void CompileProgram(Name id, const String& vertexCode, const String& fragmentCode);

	void LogShaderError(GLint shaderId);
	void LogProgramError();

	FORCEINLINE GLint FindParameterIndex(const Name& name) const
	{

		if (const auto* param = parameterIds.Find(name))
		{
			// Found cached id
			return *param;
		}
		else
		{
			// Try to find parameter on the program
			const GLint id = glGetUniformLocation(programId, name.ToString().c_str());
			if (id != GL_INVALID_INDEX)
			{
				parameterIds.Insert(name, id);
				return id;
			}
		}
		return GL_INVALID_INDEX;
	}
};
