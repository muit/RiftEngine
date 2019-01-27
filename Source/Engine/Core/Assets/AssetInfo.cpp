// © 2019 Miguel Fernández Arce - All rights reserved

#include "AssetPtr.h"
#include "Core/Serialization/Archive.h"

bool AssetInfo::Serialize(Archive& ar, const char* name)
{
	ar(name, id);
}
