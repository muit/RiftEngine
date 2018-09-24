// Copyright 2015-2019 Piperift - All rights reserved

#include "String.h"

void StringUtils::ToSentenceCase(const String& str, String& result)
{
	if (!str.empty())
	{
		//static const std::regex wordToCapital("\b[a-z]");
		static const std::regex spaceCamelCase("([a-zA-Z])(?=[A-Z])");

		result = std::regex_replace(str, spaceCamelCase, "$& ");
		result[0] = (char)::toupper(result[0]);
	}
	else
	{
		result = "";
	}
}
