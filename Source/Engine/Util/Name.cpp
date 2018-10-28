// Copyright 2015-2019 Piperift - All rights reserved

#include "Name.h"


NameTable::ConstIterator NameTable::Init(const String& string)
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
