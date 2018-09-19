// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Reflection/Reflection.h"
#include "Object/BaseObject.h"
#include "Object/ObjectPtr.h"


class Object : public BaseObject {
	CLASS(Object, BaseObject)

private:

	PROP(Ptr<BaseObject>, parent)
	Ptr<BaseObject> parent;


public:

	virtual void Construct() {}

	void SetParent(const Ptr<Object>& newParent) { parent = newParent; }
	Ptr<Object> GetParent() const { return parent.Cast<Object>(); }

	Ptr<Object> ThisPtr() { return { this }; }
};

template<typename ObjectType>
static GlobalPtr<ObjectType> Create(const Ptr<Object> parent = {}) {
	static_assert(std::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

	std::shared_ptr<ObjectType> ptr = std::make_shared<ObjectType>();
	ptr->SetParent(parent);
	ptr->Construct();

	return GlobalPtr<ObjectType>::PostCreate(std::move(ptr));
}