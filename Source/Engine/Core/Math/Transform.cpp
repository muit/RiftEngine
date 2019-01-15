// Copyright 2015-2019 Piperift - All rights reserved

#include "Transform.h"
#include "Core/Serialization/Archive.h"

bool Transform::Serialize(Archive& ar, const char* name)
{
	ar.BeginObject(name);
	{
		ar("location", location);
		ar("rotation", rotation);
		ar("scale", scale);
	}
	ar.EndObject();
	return true;
}
