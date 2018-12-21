// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/string.h>
#include <EASTL/string_view.h>
#include <regex>

#include "Memory/Allocator.h"
#include "Platform/Platform.h"

using String = eastl::basic_string<ANSICHAR, StringAllocator>;
using StringView = eastl::string_view;


namespace StringUtils {
	void ToSentenceCase(const String& str, String& result);
}

namespace eastl
{
	template <>
	struct hash<String>
	{
		size_t operator()(const String& x) const
		{
			const unsigned char* p = (const unsigned char*)x.c_str(); // To consider: limit p to at most 256 chars.
			unsigned int c, result = 2166136261U; // We implement an FNV-like String hash.
			while ((c = *p++) != 0) // Using '!=' disables compiler warnings.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};


	/// to_String
	///
	/// Converts integral types to an eastl::String with the same content that sprintf produces.  The following
	/// implementation provides a type safe conversion mechanism which avoids the common bugs associated with sprintf
	/// style format Strings.
	///
	/// http://en.cppreference.com/w/cpp/String/basic_String/to_String
	///
	inline String to_String(int value)
	{
		return String(String::CtorSprintf(), "%d", value);
	}
	inline String to_String(long value)
	{
		return String(String::CtorSprintf(), "%ld", value);
	}
	inline String to_String(long long value)
	{
		return String(String::CtorSprintf(), "%lld", value);
	}
	inline String to_String(unsigned value)
	{
		return String(String::CtorSprintf(), "%u", value);
	}
	inline String to_String(unsigned long value)
	{
		return String(String::CtorSprintf(), "%lu", value);
	}
	inline String to_String(unsigned long long value)
	{
		return String(String::CtorSprintf(), "%llu", value);
	}
	inline String to_String(float value)
	{
		return String(String::CtorSprintf(), "%f", value);
	}
	inline String to_String(double value)
	{
		return String(String::CtorSprintf(), "%f", value);
	}
	inline String to_String(long double value)
	{
		return String(String::CtorSprintf(), "%Lf", value);
	}



	/// user defined literals
	///
	/// Converts a character array literal to a basic_string.
	///
	/// Example:
	///   String s = "abcdef"s;
	///
	/// http://en.cppreference.com/w/cpp/String/basic_String/operator%22%22s
	///
	#if EASTL_USER_LITERALS_ENABLED && EASTL_INLINE_NAMESPACES_ENABLED
	EA_DISABLE_VC_WARNING(4455) // disable warning C4455: literal suffix identifiers that do not start with an underscore are reserved
	inline namespace literals
	{
		inline namespace String_literals
		{
			inline String operator"" s(const char* str, size_t len) EA_NOEXCEPT { return { str, String::size_type(len) }; }
		}
	}
	EA_RESTORE_VC_WARNING()  // warning: 4455
	#endif
} // namespace eastl
