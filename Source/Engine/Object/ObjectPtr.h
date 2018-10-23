// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/weak_ptr.h>
#include <EASTL/hash_set.h>

#include "Object/BaseObject.h"


class BaseWeakPtr;


/** Non templated version of GlobalPtr that points to all weaks */
class BaseGlobalPtr {
	friend BaseWeakPtr;

	/** MEMBERS */
	mutable eastl::hash_set<BaseWeakPtr*> weaks;

protected:

	eastl::unique_ptr<BaseObject> ptr;


	/** METHODS */

	BaseGlobalPtr() = default;
	BaseGlobalPtr(eastl::unique_ptr<BaseObject>&& inPtr) : BaseGlobalPtr() {
		ptr = eastl::move(inPtr);
	}

	BaseGlobalPtr(BaseGlobalPtr&& other) {
		MoveFrom(eastl::move(other));
	}

	BaseGlobalPtr& operator=(BaseGlobalPtr&& other) {
		MoveFrom(eastl::move(other));
		return *this;
	}

	~BaseGlobalPtr();

	void MoveFrom(BaseGlobalPtr&& other);

public:

	BaseObject* operator*()  const { return ptr.get(); }
	BaseObject* operator->() const { return ptr.get(); }

	bool IsValid() const { return ptr.get() != nullptr; }
	operator bool() const { return IsValid(); };

	void Reset() {
		if (IsValid())
		{
			ptr->Destroy();
			ptr.release();
		}
	}
};

template<typename Type>
class Ptr;


/**
 * Pointer to an Object that keeps it from being removed.
 * If a GlobalPtr goes out of scope or destroyed the object will be too.
 * GlobalPtrs are Unique. An object can't be referenced by more than one global ptr.
 */
template<typename Type>
class GlobalPtr : public BaseGlobalPtr
{
	static_assert(eastl::is_convertible< Type, BaseObject >::value, "Type is not an Object!");

	friend class GlobalPtr;


	/** METHODS */

	GlobalPtr(eastl::unique_ptr<Type>&& inPtr) : BaseGlobalPtr() { ptr = std::move(inPtr); }

public:

	GlobalPtr() = default;

	~GlobalPtr() { Reset(); }


	template<typename Type2>
	GlobalPtr(GlobalPtr<Type2>&& other) { operator=<Type2>(eastl::move(other)); }

	template<typename Type2>
	GlobalPtr& operator=(GlobalPtr<Type2>&& other) {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");

		if (!other.IsValid())
			Reset();
		else
			BaseGlobalPtr::operator=(eastl::move(other));
		return *this;
	}

	Ptr<Type> AsPtr() const { return Ptr<Type>(*this); }
	operator Ptr<Type>() const { return AsPtr(); }

	Type* operator*()  const { return static_cast<Type*>(BaseGlobalPtr::operator*());  }
	Type* operator->() const { return static_cast<Type*>(BaseGlobalPtr::operator->()); }

	template<typename Type2>
	bool operator==(const GlobalPtr<Type2>& other) const { return **this == *other; }
	template<typename Type2>
	bool operator==(const Ptr<Type2>& other) const { return **this == *other; }

	/** Cast a global pointer into another type. Will invalidate previous target when success */
	template<typename T>
	GlobalPtr<T> Cast() const {
		if (!IsValid() || dynamic_cast<Type*>(**this) == nullptr)
			return {};
		else
		{
			GlobalPtr<T> ptr{};
			ptr.MoveFrom(std::move(*this));
			return ptr;
		}
	}

	operator bool() const { return IsValid(); }

	/** INTERNAL USAGE ONLY. Use "YourObject::Create" instead */
	static GlobalPtr<Type> Create() { return { eastl::make_unique<Type>() }; }
};


/** Non templated version of GlobalPtr that points to all weak ptrs */
class BaseWeakPtr {
	friend BaseGlobalPtr;

	const BaseGlobalPtr* owner;

protected:

	BaseWeakPtr() = default;

	~BaseWeakPtr() {
		if (IsValid())
			owner->weaks.erase(this);
	}

	void Set(const BaseGlobalPtr* inOwner);

	void MoveFrom(BaseWeakPtr&& other);

	const BaseGlobalPtr* GetOwner() const { return owner; }

public:

	bool IsValid() const { return owner != nullptr && owner->IsValid(); }
	operator bool() const { return IsValid(); };

	void Reset() {
		// Remove old weak ptr
		if(owner)
			owner->weaks.erase(this);
		CleanOwner();
	}

private:

	void CleanOwner() {
		owner = nullptr;
	}
};


/**
* Weak Object Pointers
* Objects will be removed if their global ptr is destroyed. In this case all pointers will be invalidated.
*/
template<typename Type>
class Ptr : public BaseWeakPtr
{
	static_assert(eastl::is_convertible<Type, BaseObject>::value, "Type is not an Object!");

	friend GlobalPtr<Type>;
	friend class Ptr;


	/** METHODS */

public:

	Ptr() = default;

	/** Templates for down-casting */
	template<typename Type2>
	Ptr(const Ptr<Type2>& other) : BaseWeakPtr{} {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");
		Set(other.GetOwner());
	}
	template<typename Type2>
	Ptr& operator=(const Ptr<Type2>& other) {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");
		Set(other.GetOwner());
		return *this;
	};

	template<typename Type2>
	Ptr(Ptr<Type2>&& other) : BaseWeakPtr{} {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");
		MoveFrom(eastl::move(other));
	}
	template<typename Type2>
	Ptr& operator=(Ptr<Type2>&& other) {
		static_assert(eastl::is_convertible<Type2, Type>::value, "Type is not compatible!");
		MoveFrom(eastl::move(other));
		return *this;
	}

	template<typename Type2>
	Ptr(const GlobalPtr<Type2>& other) : BaseWeakPtr{} {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");
		Set(&other);
	}

	template<typename Type2>
	Ptr(Type2* other) : BaseWeakPtr{} {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");
		if (!other) {
			Reset();
			return;
		}
		Ptr<Type2>(other->GetSelf());
	}


	template<typename Type2>
	Ptr& operator=(const GlobalPtr<Type2>& other) {
		Set(&other);
		return *this;
	};

	Ptr& operator=(TYPE_OF_NULLPTR) {
		Reset();
		return *this;
	};


	template<typename Type2>
	bool operator==(const Ptr<Type2>& other) const {
		return GetOwner() == other.GetOwner();
	}

	template<typename Type2>
	bool operator==(const GlobalPtr<Type2>& other) const { return other == *this; }

	Type* operator*() const {
		// Static cast since types are always cast-able or invalid
		return IsValid() ? static_cast<Type*>(GetOwner()->operator*()) : nullptr;
	}
	Type* operator->() const {
		// Static cast since types are always cast-able or invalid
		return IsValid() ? static_cast<Type*>(GetOwner()->operator->()) : nullptr;
	}

	template<typename T>
	Ptr<T> Cast() const {
		if (!IsValid() || dynamic_cast<Type*>(**this) == nullptr)
			return {};
		else
		{
			Ptr<T> ptr{};
			ptr.Set(GetOwner());
			return ptr;
		}
	}
};

