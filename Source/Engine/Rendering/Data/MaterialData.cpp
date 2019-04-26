// � 2019 Miguel Fern�ndez Arce - All rights reserved

#include "MaterialData.h"

bool MaterialData::Parse(const String& code)
{
	static const char* fragmentDelimiter = "## FRAGMENT ##";
	static const char* vertexDelimiter = "## VERTEX ##";

	String before, after;
	// Contains vertex shader?
	if (CString::Split(code, before, after, vertexDelimiter))
	{
		String a, b;

		// Is Fragment before Vertex?
		if (CString::Split(before, a, b, fragmentDelimiter))
		{
			vertexCode = b;
			fragmentCode = a;
		}
		// Is Fragment after Vertex?
		else if (CString::Split(after, a, b, fragmentDelimiter))
		{
			vertexCode = a;
			fragmentCode = b;
		}
		else
		{
			vertexCode = after;
		}
	}
	// Contains Fragment shader?
	else if (CString::Split(code, before, after, fragmentDelimiter))
	{
		fragmentCode = after;
	}
	else
	{
		return false;
	}

	CString::RemoveFromStart(fragmentCode, std::strlen(fragmentDelimiter));
	CString::RemoveFromStart(vertexCode, std::strlen(vertexDelimiter));

	return true;
}
