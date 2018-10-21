// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/weak_ptr.h>

#include "Object/BaseObject.h"


template<typename Type>
class Ptr;

/**
 * Pointer to an Object that keeps it from being removed.
 * If a GlobalPtr goes out of scope or destroyed the object will be too.
 * GlobalPtrs are Unique. An object can't be referenced by more than one global ptr.
 */
template<typename Type>
class GlobalPtr
{
	static_assert(eastl::is_convertible< Type, BaseObject >::value, "Type is not an Object!");

	friend class GlobalPtr;
	friend GlobalPtr<Type> GlobalPtr<Type>::PostCreate(eastl::shared_ptr<Type>&&);


public:

	GlobalPtr() = default;
	GlobalPtr(const GlobalPtr&) = delete;
	GlobalPtr(GlobalPtr&& other) {
		ptr = eastl::move(other.ptr);
	}
	GlobalPtr& operator=(const GlobalPtr&) = delete;
	GlobalPtr& operator=(GlobalPtr&& other) {
		ptr = eastl::move(other.ptr);
		return *this;
	}
	~GlobalPtr() {
		// Force memory deallocation
		if (IsValid())
			ptr->Destroy();
		ptr.reset();
	}


	template<typename Type2>
	GlobalPtr(GlobalPtr<Type2>&& other) { operator=<Type2>(eastl::move(other)); }

	template<typename Type2>
	GlobalPtr& operator=(GlobalPtr<Type2>&& other) {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");

		if (!other)
			ptr = nullptr;
		else
		{
			ptr = other.Cast<Type>().ptr;
			other.Destroy();
		}
		return *this;
	}

	Ptr<Type> GetPtr() const { return { ptr }; }
	operator Ptr<Type>() const { return GetPtr(); }
	Ptr<Type> operator->() const { return GetPtr(); }

	template<typename Type2>
	bool operator==(const GlobalPtr<Type2>& other) const { return ptr == other.ptr; }
	template<typename Type2>
	bool operator==(const Ptr<Type2>& other) const { return ptr == other.ptr.lock(); }

	template<typename T>
	GlobalPtr<T> Cast() const { return { eastl::dynamic_pointer_cast<T>(ptr) }; }

	/** Manual destruction */
	void Destroy() { ptr = nullptr; }

	bool IsValid() const { return !!ptr.get(); }
	operator bool() const { return IsValid(); };


	/** Internal usage only */
	static GlobalPtr<Type> PostCreate(eastl::shared_ptr<Type>&& inPtr)
	{ return { eastl::move(inPtr) }; }

private:

	GlobalPtr(eastl::shared_ptr<Type>&& inPtr) { ptr = inPtr; }

	eastl::shared_ptr<Type> ptr;
};


/**
* Weak Object Pointers
* Objects will be removed if their global ptr is destroyed. In this case all pointers will be invalidated.
*/
template<typename Type>
class Ptr
{
	static_assert(eastl::is_convertible< Type, BaseObject >::value, "Type is not an Object!");

	friend GlobalPtr<Type>;
	friend class Ptr;


private:

	Ptr(eastl::weak_ptr<Type>&& ptr) : ptr(ptr) {}

public:

	Ptr() = default;

	Ptr(const Ptr<Type>& other) { operator=(other); }
	Ptr(Ptr<Type>&& other) { operator=(eastl::move(other)); }

	template<typename Type2>
	Ptr(const Ptr<Type2>& other) { operator=(other); }
	template<typename Type2>
	Ptr(Ptr<Type2>&& other) { operator=(eastl::move(other)); }

	template<typename Type2>
	Ptr(Type2* other) {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");
		if (!other) {
			ptr = {};
			return;
		}
		Ptr(eastl::weak_ptr<Type>{ eastl::dynamic_pointer_cast<Type>(other->shared_from_this()) });
	}

	Ptr& operator=(const Ptr& other) {
		ptr = other.ptr;
		return *this;
	};

	Ptr& operator=(Ptr&& other) {
		ptr = other.ptr;
		other.Invalidate();
		return *this;
	}

	/** We use templates for down-casting */
	template<typename Type2>
	Ptr& operator=(const Ptr<Type2>& other) {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");
		ptr = other.Cast<Type>().ptr;
		return *this;
	};

	template<typename Type2>
	Ptr& operator=(Ptr<Type2>&& other) {
		static_assert(eastl::is_convertible< Type2, Type >::value, "Type is not compatible!");
		ptr = other.Cast<Type>().ptr;
		other.Invalidate();
		return *this;
	}


	template<typename Type2>
	Ptr& operator=(const GlobalPtr<Type2>& other) {
		*this = eastl::move(other.GetPtr());
		return *this;
	};
	Ptr& operator=(TYPE_OF_NULLPTR) {
		ptr = {};
		return *this;
	};

	template<typename Type2>
	bool operator==(const Ptr<Type2>& other) const {
		return this->ptr.lock() == other.ptr.lock();
	}

	template<typename Type2>
	bool operator==(const GlobalPtr<Type2>& other) const { return other == *this; }

	bool IsValid() const { return !ptr.expired(); }
	operator bool() const { return IsValid(); };

	Type* operator*() const {
		return IsValid() ? ptr.lock().get() : nullptr;
	}
	Type* operator->() const { return operator*(); }

	template<typename T>
	Ptr<T> Cast() const {
		return { eastl::dynamic_pointer_cast<T>(ptr.lock()) };
	}

	void Invalidate() { ptr = {}; }

private:

	eastl::weak_ptr<Type> ptr;
};

