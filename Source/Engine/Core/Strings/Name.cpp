// Copyright 2015-2019 Piperift - All rights reserved

#include "Name.h"
#include "Core/Serialization/Archive.h"


NameTable::ConstIterator NameTable::Init(const String& string)
{
	if (string.empty())
		return None();

	ConstIterator FoundIt = table.find(string);
	if (FoundIt != None())
		return FoundIt;
	else
	{
		return table.insert(string).first;
	}
}


const String Name::NoneStr {""};

bool Name::Serialize(Archive& ar, const char* name)
{
	if (ar.IsSaving())
	{
		String str = ToString();
		ar(name, str);
	}
	else
	{
		String str;
		ar(name, str);
		*this = str;
	}
	return true;
}
