// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/string.h>
#include <EASTL/string_view.h>
#include <regex>

typedef eastl::string String;
typedef eastl::string_view StringView;

namespace StringUtils {
	void ToSentenceCase(const String& str, String& result);
}
