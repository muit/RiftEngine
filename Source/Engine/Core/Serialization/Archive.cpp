// Copyright 2015-2019 Piperift - All rights reserved

#include "Archive.h"

void Archive::Serialize(const char* name, v2& val)
{
	BeginObject(name);
	{
		Serialize(TX("x"), val.x);
		Serialize(TX("y"), val.y);
	}
	EndObject();
}

void Archive::Serialize(const char* name, v2_u32& val)
{
	BeginObject(name);
	{
		Serialize(TX("x"), val.x);
		Serialize(TX("y"), val.y);
	}
	EndObject();
}

void Archive::Serialize(const char* name, v3& val)
{
	BeginObject(name);
	{
		Serialize(TX("x"), val.x);
		Serialize(TX("y"), val.y);
		Serialize(TX("z"), val.z);
	}
	EndObject();
}

void Archive::Serialize(const char* name, Quat& val)
{
	BeginObject(name);
	{
		Serialize(TX("x"), val.x);
		Serialize(TX("y"), val.y);
		Serialize(TX("z"), val.z);
		Serialize(TX("w"), val.w);
	}
	EndObject();
}

void JsonArchive::Serialize(const char* name, bool& val)
{
	if (IsLoading())
	{
		const Json& field = Data()[name];
		val = field.is_boolean() ? field.get<bool>() : false;
	}
	else
	{
		Data()[name] = val;
	}
}

void JsonArchive::Serialize(const char* name, u8& val)
{
	if (IsLoading())
	{
		const Json& field = Data()[name];
		val = field.is_number_unsigned() ? field.get<u8>() : 0;
	}
	else
	{
		Data()[name] = val;
	}
}

void JsonArchive::Serialize(const char* name, i32& val)
{
	if (IsLoading())
	{
		const Json& field = Data()[name];
		val = field.is_number_integer() ? field.get<i32>() : 0;
	}
	else
	{
		Data()[name] = val;
	}
}

void JsonArchive::Serialize(const char* name, u32& val)
{
	if (IsLoading())
	{
		const Json& field = Data()[name];
		val = field.is_number_unsigned() ? field.get<u32>() : 0;
	}
	else
	{
		Data()[name] = val;
	}
}

void JsonArchive::Serialize(const char* name, float& val)
{
	if (IsLoading())
	{
		const Json& field = Data()[name];
		val = field.is_number_float() ? field.get<float>() : 0.f;
	}
	else
	{
		Data()[name] = val;
	}
}

void JsonArchive::Serialize(const char* name, String& val)
{
	if (IsLoading())
	{
		const Json& field = Data()[name];
		val = field.is_string() ? field.get<String>() : String{};
	}
	else
	{
		Data()[name] = val;
	}
}

void JsonArchive::Serialize(const char* name, Json& val)
{
	if (IsLoading())
	{
		val = Data()[name];
	}
	else
	{
		Data()[name] = val;
	}
}
