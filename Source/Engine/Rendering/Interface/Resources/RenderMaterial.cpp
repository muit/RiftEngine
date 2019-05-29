// © 2019 Miguel Fernández Arce - All rights reserved

#include "RenderMaterial.h"
#include "../../Resources.h"
#include "../OpenGL.h"


const Name RenderMaterial::notAResourceId { "Not a Resource" };

void RenderMaterial::CompileProgram(Name id, const String& vertexCode, const String& fragmentCode)
{
	GLint success = GL_FALSE;

	// Create shader objects
	GLuint   vertexId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader code
	const char* vertexCodeCStr[1] = { vertexCode.c_str() };
	const char* fragmentCodeCStr[1] = { fragmentCode.c_str() };
	const GLint vertexSize[1] = { (GLint)vertexCode.size() };
	const GLint fragmentSize[1] = { (GLint)fragmentCode.size() };

	glShaderSource(vertexId, 1, vertexCodeCStr, vertexSize);
	glShaderSource(fragmentId, 1, fragmentCodeCStr, fragmentSize);

	// Compile shaders
	glCompileShader(vertexId);
	glCompileShader(fragmentId);


	bool bError = false;
	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		LogShaderError(vertexId);
		bError = true;
	}

	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		LogShaderError(fragmentId);
		bError |= true;
	}

	if (!bError)
	{
		// Link shaders into a new program
		programId = glCreateProgram();
		glAttachShader(programId, vertexId);
		glAttachShader(programId, fragmentId);
		glLinkProgram(programId);

		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success)
		{
			LogProgramError();
		}
		else
		{
			Log::Message("Loaded material into GPU '%s' Id:%i", id.ToString().c_str(), programId);
		}
	}

	// Free compiled shaders
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	glCheckError();
}

void RenderMaterial::BindStaticParameters(const Resources& resources) const
{
	for (i32 i = 0; i < textures.Size(); ++i)
	{
		const auto& texture = textures[i];

		const GLint id = FindParameterIndex(texture.name);
		if (id != GL_INVALID_INDEX)
		{
			//https://community.khronos.org/t/passing-multiple-textures-from-opengl-to-glsl-shader/53096
			const RenderTexture& textureRes = resources.Get<ResourceType::Texture>(texture.asset.GetPath());
			glActiveTexture(GL_TEXTURE0 + i);
			textureRes.Bind();

			glUniform1i(id, i);
		}
	}
}

void RenderMaterial::LogShaderError(GLint shaderId)
{
	String message;
	GLint messageSize;

	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &messageSize);
	message.resize(messageSize);
	glGetShaderInfoLog(shaderId, messageSize, nullptr, &message.front());

	Log::Error(message);
}

void RenderMaterial::LogProgramError()
{
	String message;
	GLint messageSize;

	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &messageSize);
	message.resize(messageSize);
	glGetProgramInfoLog(programId, messageSize, nullptr, &message.front());

	Log::Error(message);
}

bool RenderMaterial::SetFloat(Name name, float value) const
{
	const GLint id = FindParameterIndex(name);
	if (id != GL_INVALID_INDEX)
	{
		glUniform1f(id, value);
		return true;
	}
	return false;
}

bool RenderMaterial::SetI32(Name name, i32 value) const
{
	const GLint id = FindParameterIndex(name);
	if (id != GL_INVALID_INDEX)
	{
		glUniform1i(id, value);
		return true;
	}
	return false;
}

bool RenderMaterial::SetV3(Name name, const v3& value) const
{
	const GLint id = FindParameterIndex(name);
	if (id != GL_INVALID_INDEX)
	{
		glUniform3fv(id, 1, value.Data());
		return true;
	}
	return false;
}

bool RenderMaterial::SetMatrix4f(Name name, const Matrix4f& value) const
{
	const GLint id = FindParameterIndex(name);
	if (id != GL_INVALID_INDEX)
	{
		glUniformMatrix4fv(id, 1, GL_FALSE, value.Data());
		return true;
	}
	return false;
}

GLint RenderMaterial::FindParameterIndex(const Name& name) const
{
	if (const auto * param = parameterIds.Find(name))
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
