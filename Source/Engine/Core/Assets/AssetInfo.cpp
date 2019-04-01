// © 2019 Miguel Fernández Arce - All rights reserved

#include "AssetInfo.h"
#include "Core/Serialization/Archive.h"

bool AssetInfo::Serialize(class Archive& ar, const char* name)
{
	ar(name, id);
	return true;
}
