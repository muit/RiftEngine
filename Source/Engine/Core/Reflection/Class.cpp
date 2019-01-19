// Copyright 2015-2019 Piperift - All rights reserved

#include "Class.h"

Class* Class::FindChildClass(const Name& className) const
{
	if (className.IsNone())
		return nullptr;

	for (auto* child : children)
	{
		if (child->GetName() == className)
			return child;
		else if (Class* found = child->FindChildClass(className))
			return found;
	}
	return nullptr;
}

bool Class::IsChildOf(Class* other) const
{
	if (!other || !parent)
		return false;

	const Class* tempParent = parent;
	while (tempParent)
	{
		if (other == tempParent)
			return true;

		tempParent = tempParent->parent;
	}
	return false;
}
