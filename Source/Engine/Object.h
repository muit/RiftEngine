// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Object/BaseObject.h"
#include "Object/ObjectPtr.h"
#include "Reflection/TClass.h"


class Object;

template<typename ObjectType>
static GlobalPtr<ObjectType> Create(const Ptr<Object> parent = {}) {
	static_assert(std::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

	std::shared_ptr<ObjectType> ptr = std::make_shared<ObjectType>();
	ptr->ownClass = ObjectType::StaticClass();
	ptr->SetOwner(parent);
	ptr->Construct();

	return GlobalPtr<ObjectType>::PostCreate(std::move(ptr));
}


class Object : public BaseObject {
	ORPHAN_CLASS(Object)

	template<typename ObjectType>
	friend GlobalPtr<ObjectType> Create(const Ptr<Object>);

private:

	Class* ownClass;
	Ptr<BaseObject> owner;


public:

	virtual void Construct() {}

	void SetOwner(const Ptr<Object>& newOwner) { owner = newOwner; }
	Ptr<Object> GetOwner() const { return owner.Cast<Object>(); }

	Ptr<Object> ThisPtr() { return { this }; }

	Class* GetClass() const { return ownClass; }
};
