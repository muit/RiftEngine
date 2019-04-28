// © 2019 Miguel Fernández Arce - All rights reserved

#include "RenderMaterial.h"

void RenderMaterial::CompileProgram(const String& vertexCode, const String& fragmentCode)
{
	GLint state = GL_FALSE;

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

	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &state);
	if (!state)
		LogShaderError(vertexId);

	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &state);
	if (!state)
		LogShaderError(fragmentId);

	// Link shaders into a new program
	programId = glCreateProgram();
	glAttachShader(programId, vertexId);
	glAttachShader(programId, fragmentId);
	glLinkProgram(programId);

	glGetProgramiv(programId, GL_LINK_STATUS, &state);
	if (!state)
		LogProgramError();

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
	glGetShaderInfoLog(shaderId, messageSize, NULL, &message.front());

	Log::Error(message);
}

void RenderMaterial::LogProgramError()
{
	String message;
	GLint messageSize;

	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &messageSize);
	message.resize(messageSize);
	glGetProgramInfoLog(programId, messageSize, NULL, &message.front());

	Log::Error(message);
}

bool RenderMaterial::SetFloat(Name Id, float value) const
{
	GLuint paramId = glGetUniformLocation(programId, Id.ToString().c_str());
	if (paramId != GL_INVALID_INDEX)
	{
		glUniform1fv(paramId, 1, &value);
		return true;
	}
	return false;
}

bool RenderMaterial::SetV3(Name Id, v3 value) const
{
	GLuint paramId = glGetUniformLocation(programId, Id.ToString().c_str());
	if (paramId != GL_INVALID_INDEX)
	{
		glUniform3fv(paramId, 1, value.data());
		return true;
	}
	return false;
}

bool RenderMaterial::SetMatrix4f(Name Id, Matrix4f value) const
{
	GLuint paramId = glGetUniformLocation(programId, Id.ToString().c_str());
	if (paramId != GL_INVALID_INDEX)
	{
		glUniformMatrix4fv(paramId, 1, GL_FALSE, value.data());
		return true;
	}
	return false;
}