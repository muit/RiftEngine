// Copyright 2015-2019 Piperift - All rights reserved

#include "String.h"
#include "Core/Misc/Char.h"
#include "../Math.h"

void CString::ToSentenceCase(const String& str, String& result)
{
	if (!str.empty())
	{
		//static const std::regex wordToCapital("\b[a-z]");
		static const Regex spaceCamelCase("([a-zA-Z])(?=[A-Z])");

		result = String{ std::regex_replace(str.c_str(), spaceCamelCase, TEXT("$& ")).c_str() };
		result[0] = (char)::toupper(result[0]);
	}
	else
	{
		result = "";
	}
}

int32 CString::ParseIntoArray(const String& str, TArray<String>& OutArray, const TCHAR* pchDelim, bool InCullEmpty /*= true*/)
{
	return 0;
}

String CString::BackSubstr(const String& str, int32 size)
{
	return str.substr(Math::Max(0u, str.size() - size), size);
}

bool CString::IsNumeric(const TCHAR* Str)
{
	if (*Str == '-' || *Str == '+')
	{
		Str++;
	}

	bool bHasDot = false;
	while (*Str != '\0')
	{
		if (*Str == '.')
		{
			if (bHasDot)
			{
				return false;
			}
			bHasDot = true;
		}
		else if (!FChar::IsDigit(*Str))
		{
			return false;
		}

		++Str;
	}

	return true;
}

//const std::wstring_convert<std::codecvt_utf8<WIDECHAR>, WIDECHAR, StringAllocator, StringAllocator> CString::converter {};
