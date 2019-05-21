// Copyright 2015-2019 Piperift - All rights reserved

#include <cstdio>
#include <iostream>
#include "Core/Strings/Name.h"

#include <Core/Engine.h>


int main(int, char**)
{
	Name a{ "ss" };
	Name b{ "ss" };

	bool ab = a == b;

	Name c{ "" };
	Name d{ "" };

	bool cd = c == d;

	bool av = a.IsNone();
	bool cv = c.IsNone();

	Name e;
	e = a;
	String as = a.ToString();
	String cs = c.ToString();
	String es = e.ToString();

	const bool bSuccess = Engine::StartEngine();
	return !bSuccess;
}
