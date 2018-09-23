// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <string>
#include <vector>

#include "EngineTypes.h"
#include "Object/BaseObject.h"
#include "Serialization/Archive.h"
#include "Runtime/PropertyHandle.h"


/**
* Static information about a property
*/
class Property {
private:

	Name name;
	std::vector<Name> tags;


	Property() = delete;
	Property(Property&&) = delete;
	Property(const Property&) = delete;

protected:

	Property(Name&& name, std::vector<Name>&& tags)
		: name(name), tags(tags)
	{}

public:

	virtual ~Property() = default;

	//	virtual void Serialize(void* instance, Archive& archive) const {}

	String GetName() { return name.ToString(); }
	bool HasTag(Name tag) const { return std::find(tags.begin(), tags.end(), std::move(tag)) != tags.end(); }
};
