// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"

#include <EASTL/vector.h>

#include "BaseType.h"
#include "Object/BaseObject.h"


class Class : public BaseType {
protected:

	Class* parent;
	eastl::vector<Class*> children;

public:

	virtual GlobalPtr<BaseObject> CreateInstance(const Ptr<BaseObject>& owner) = 0;

	Class* GetParentClass() const { return parent; }

	template<bool bIncludeSelf = false, bool bIsFirstCall = true>
	void GetAllChildClasses(eastl::vector<Class*>& outChildren) {
		if(bIsFirstCall)
			outChildren.clear();

		if (bIncludeSelf)
			outChildren.push_back(this);

		for (auto* child : children)
		{
			outChildren.push_back(child);
			child->GetAllChildClasses<false, false>(outChildren);
		}
	}

public:

	/** GENERATION */
	void RegistryChildren(Class* child)
	{
		children.push_back(child);
	}
};
