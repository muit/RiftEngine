// © 2019 Miguel Fernández Arce - All rights reserved

#include "RenderMaterial.h"

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
			// Cache parameter ids by name
			parameterIds.Add(
				{ "mvp", glGetUniformLocation(programId, "mvp") }
			);

			Log::Message("Loaded material into GPU '%s' Id:%i", id.ToString().c_str(), programId);
		}
	}

	// Free compiled shaders
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
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

bool RenderMaterial::SetFloat(Name Id, float value) const
{
	const GLint paramId = glGetUniformLocation(programId, Id.ToString().c_str());
	if (paramId != GL_INVALID_INDEX)
	{
		glUniform1fv(paramId, 1, &value);
		return true;
	}
	return false;
}

bool RenderMaterial::SetV3(Name Id, const v3& value) const
{
	const GLint paramId = glGetUniformLocation(programId, Id.ToString().c_str());
	if (paramId != GL_INVALID_INDEX)
	{
		glUniform3fv(paramId, 1, value.Data());
		return true;
	}
	return false;
}

bool RenderMaterial::SetMatrix4f(Name Id, const Matrix4f& value) const
{
	const GLint paramId = glGetUniformLocation(programId, Id.ToString().c_str());
	if (paramId != -1)
	{
		glUniformMatrix4fv(paramId, 1, GL_FALSE, value.Data());
		return true;
	}
	return false;
}
