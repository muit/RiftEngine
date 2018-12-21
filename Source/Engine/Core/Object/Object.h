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
static GlobalPtr<ObjectType> Create(Class* objectClass, const Ptr<Object> owner = {}) {
	static_assert(eastl::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

	if (objectClass)
	{
		return objectClass->CreateInstance(owner).Cast<ObjectType>();
	}
	return {};
}

template<typename ObjectType>
static GlobalPtr<ObjectType> Create(const Ptr<BaseObject> owner = {}) {
	static_assert(eastl::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

	GlobalPtr<ObjectType> ptr = GlobalPtr<ObjectType>::Create(owner);
	return ptr;
}


class Object : public BaseObject {
	ORPHAN_CLASS(Object)

private:

	PROP(Name, name)
	Name name;

	Ptr<BaseObject> self;
	Class* ownClass;
	Ptr<BaseObject> owner;


public:

	NOINLINE Object() : BaseObject(), self{}, ownClass{ nullptr }, owner{} {
	};

	NOINLINE void PreConstruct(Ptr<BaseObject>&& inSelf, Class* inClass, const Ptr<BaseObject>& inOwner) {
		ownClass = inClass; self = eastl::move(inSelf); owner = inOwner;
	}
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
