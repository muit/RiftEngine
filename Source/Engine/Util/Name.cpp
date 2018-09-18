// Copyright 2015-2019 Piperift - All rights reserved

#include "Name.h"


NameTable::ConstIterator NameTable::Init(const ANSICHAR* string)
{
	if (std::strlen(string) == 0)
		return None();

	ConstIterator FoundIt = table.find(string);
	if (FoundIt != None())
		return FoundIt;
	else
	{
		return table.insert(string).first;
	}
}

NameTable::ConstIterator NameTable::Init(const std::basic_string<ANSICHAR>& string)
{
	if (string.size() == 0)
		return None();

	ConstIterator FoundIt = table.find(string);
	if (FoundIt != None())
		return FoundIt;
	else
	{
		return table.insert(string).first;
	}
}
