// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"
#include "Object/BaseObject.h"
#include "EngineTypes.h"
#include "Object/ObjectPtr.h"
#include "Reflection/TClass.h"


class Object;
class World;

template<typename ObjectType>
static GlobalPtr<ObjectType> Create(const Ptr<Object> parent = {}) {
	static_assert(eastl::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

	GlobalPtr<ObjectType> ptr{ GlobalPtr<ObjectType>::Create() };
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

	PROP(Name, name)
	Name name;

	Ptr<BaseObject> self;
	Class* ownClass;
	Ptr<BaseObject> owner;


public:

	Object() : BaseObject(), self{}, ownClass{ nullptr }, owner{} {};

	virtual void Construct() {}

	virtual bool Serialize(Archive& ar) {
		SerializeReflection(ar);
		return true;
	}


	Ptr<Object> GetOwner() const { return owner.Cast<Object>(); }

	Ptr<Object> GetSelf() const { return self.Cast<Object>(); }

	Class* GetClass() const { return ownClass; }

	Name GetName() { return name; }

	virtual Ptr<World> GetWorld() const;
};
