// Copyright 2015-2019 Piperift - All rights reserved

#include "Name.h"
#include "Core/Serialization/Archive.h"


const String Name::noneStr{ "none" };
const NameKey Name::noneId{ 0 };

NameKey NameTable::Register(const String& string)
{
	if (string.empty())
		return Name::noneId;

	// Calculate hash once
	NameKey key{ string };

	ConstIterator FoundIt = table.find(key);
	if (FoundIt != table.end())
	{
		std::shared_lock lock{ editTableMutex };
		return *FoundIt;
	}
	else
	{
		std::unique_lock lock{ editTableMutex };
		return *table.insert(MoveTemp(key)).first;
	}
}

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
