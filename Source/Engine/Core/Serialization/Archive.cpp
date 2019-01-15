// Copyright 2015-2019 Piperift - All rights reserved

#include "Archive.h"

void Archive::Serialize(const char* name, v2& val)
{
	BeginObject(name);
	{
		Serialize(TX("x"), val.x());
		Serialize(TX("y"), val.y());
	}
	EndObject();
}

void Archive::Serialize(const char* name, v2_u32& val)
{
	BeginObject(name);
	{
		Serialize(TX("x"), val.x());
		Serialize(TX("y"), val.y());
	}
	EndObject();
}

void Archive::Serialize(const char* name, v3& val)
{
	BeginObject(name);
	{
		Serialize(TX("x"), val.x());
		Serialize(TX("y"), val.y());
		Serialize(TX("z"), val.z());
	}
	EndObject();
}

void Archive::Serialize(const char* name, Quat& val)
{
	BeginObject(name);
	{
		Serialize(TX("x"), val.x());
		Serialize(TX("y"), val.y());
		Serialize(TX("z"), val.z());
		Serialize(TX("w"), val.w());
	}
	EndObject();
}
