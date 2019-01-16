 // Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Core/Object/BaseObject.h"
#include "Core/Object/ObjectPtr.h"
#include "../Property.h"


#if WITH_EDITOR
class PropertyWidget;
#endif


struct PropertyHandle {
protected:

	const Ptr<BaseObject> instance;
	const Property* const prop;


	PropertyHandle(const Ptr<BaseObject>& instance, const Property* prop) : instance{ instance }, prop{ prop } {}

public:

	virtual ~PropertyHandle() {}


	FORCEINLINE String GetName() const
	{
		if (prop)
			return prop->GetName();
		return "Invalid";
	}

	FORCEINLINE bool HasTag(ReflectionTags tag) const {
		return prop ? prop->HasTag(tag) : false;
	}

	bool IsValid() const { return instance && prop != nullptr; }

	operator bool() const { return IsValid(); }


	FORCEINLINE virtual Class* GetClassDefinedWidgetClass() {
		return nullptr;
	}
};
