// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"
#include "BaseType.h"


class StructType : public BaseType {
protected:

	StructType* parent;
	TArray<StructType*> children;

public:

	StructType* GetParent() const { return parent; }

	template<bool bIncludeSelf = false, bool bIsFirstCall = true>
	void GetAllChildren(TArray<StructType*>& outChildren) {
		if (bIsFirstCall)
			outChildren.Empty();

		if (bIncludeSelf)
			outChildren.Add(this);

		for (auto* const child : children)
		{
			outChildren.Add(child);
			child->GetAllChildren<false, false>(outChildren);
		}
	}

	StructType* FindChild(const Name& className) const {
		if (className.IsNone())
			return nullptr;

		for (auto* const child : children)
		{
			if (child->GetName() == className)
				return child;
			else if (StructType* found = child->FindChild(className))
				return found;
		}
		return nullptr;
	}

public:

	/** GENERATION */
	void RegistryChild(StructType* child)
	{
		children.Add(child);
	}
};
