// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Object/BaseObject.h"
#include "EngineTypes.h"
#include "Object/ObjectPtr.h"
#include "Reflection/TClass.h"


class Object;
class World;

template<typename ObjectType>
static GlobalPtr<ObjectType> Create(const Ptr<Object> parent = {}) {
	static_assert(eastl::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

	GlobalPtr<ObjectType> ptr = GlobalPtr<ObjectType>::Create();
	ptr->ownClass = ObjectType::StaticClass();
	ptr->self = ptr;
	ptr->owner = parent;
	ptr->Construct();

	return eastl::move(ptr);
}


class Object : public BaseObject {
	ORPHAN_CLASS(Object)

	template<typename ObjectType>
	friend GlobalPtr<ObjectType> Create(const Ptr<Object>);

private:

	Ptr<BaseObject> self;
	Class* ownClass;
	Ptr<BaseObject> owner;


public:

	virtual void Construct() {}

	Ptr<Object> GetOwner() const { return owner.Cast<Object>(); }

	Ptr<Object> GetSelf() const { return self.Cast<Object>(); }

	Class* GetClass() const { return ownClass; }

	virtual Ptr<World> GetWorld() const;
};
