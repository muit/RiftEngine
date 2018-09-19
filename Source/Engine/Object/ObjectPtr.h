// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Object/BaseObject.h"


template<typename Type>
class Ptr;

template<typename Type>
class GlobalPtr
{
	static_assert(std::is_convertible< Type, BaseObject >::value, "Type is not an Object!");

	friend class GlobalPtr;
	friend GlobalPtr<Type> GlobalPtr<Type>::PostCreate(std::shared_ptr<Type>&&);


public:

	GlobalPtr() = default;
	GlobalPtr(const GlobalPtr&) = delete;
	GlobalPtr(GlobalPtr&& other) {
		ptr = std::move(other.ptr);
	}
	GlobalPtr& operator=(const GlobalPtr&) = delete;
	GlobalPtr& operator=(GlobalPtr&& other) {
		ptr = std::move(other.ptr);
		return *this;
	}
	~GlobalPtr() {
		// Force memory deallocation
		if (IsValid())
			ptr->BeforeDestroy();
		ptr.reset();
	}


	template<typename Type2>
	GlobalPtr(GlobalPtr<Type2>&& other) { operator<Type2>(other); }

	template<typename Type2>
	GlobalPtr& operator=(GlobalPtr<Type2>&& other) {
		static_assert(std::is_convertible< Type2, Type >::value, "Type is not compatible!");

		if (!other)
			ptr = nullptr;
		else
		{
			ptr = other.Cast<Type>().ptr;
			other.Destroy();
		}
		return *this;
	}

	operator Ptr<Type>() const {
		return { *this };
	}
	Ptr<Type> GetPtr() const { return { ptr }; }
	Ptr<Type> operator->() const {
		return GetPtr();
	}

	template<typename Type2>
	bool operator==(const GlobalPtr<Type2>& other) const { return ptr == other.ptr; }
	template<typename Type2>
	bool operator==(const Ptr<Type2>& other) const { return ptr == other.ptr.lock(); }

	template<typename T>
	GlobalPtr<T> Cast() const { return { std::dynamic_pointer_cast<T>(ptr) }; }

	/** Manual destruction */
	void Destroy() { ptr = nullptr; }

	bool IsValid() const { return !!ptr.get(); }
	operator bool() const { return IsValid(); };


	/** Internal usage only */
	static GlobalPtr<Type> PostCreate(std::shared_ptr<Type>&& inPtr)
	{ return { std::move(inPtr) }; }

private:

	GlobalPtr(std::shared_ptr<Type>&& inPtr) { ptr = inPtr; }

	std::shared_ptr<Type> ptr;
};


template<typename Type>
class Ptr
{
	static_assert(std::is_convertible< Type, BaseObject >::value, "Type is not an Object!");

	friend GlobalPtr<Type>;
	friend class Ptr;


private:

	Ptr(std::weak_ptr<Type>&& ptr) : ptr(ptr) {}

public:

	Ptr() = default;

	template<typename Type2>
	Ptr(const Ptr<Type2>& other) { operator=(other); }
	template<typename Type2>
	Ptr(Ptr<Type2>&& other) { operator=(std::move(other)); }

	template<typename Type2>
	Ptr(Type2* other) {
		static_assert(std::is_convertible< Type2, Type >::value, "Type is not compatible!");
		if (!other) {
			ptr = {};
			return;
		}
		Ptr(std::weak_ptr<Type>{ std::dynamic_pointer_cast<Type>(other->shared_from_this()) });
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
		static_assert(std::is_convertible< Type2, Type >::value, "Type is not compatible!");
		ptr = other.Cast<Type>().ptr;
		return *this;
	};

	template<typename Type2>
	Ptr& operator=(Ptr<Type2>&& other) {
		static_assert(std::is_convertible< Type2, Type >::value, "Type is not compatible!");
		ptr = other.Cast<Type>().ptr;
		other.Invalidate();
		return *this;
	}


	template<typename Type2>
	Ptr& operator=(const GlobalPtr<Type2>& other) {
		*this = std::move(other.GetPtr());
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

	Type* operator->() const {
		return ptr.expired()? nullptr : ptr.lock().get();
	}

	bool IsValid() const { return !ptr.expired(); }
	operator bool() const {	return IsValid(); };

	template<typename T>
	Ptr<T> Cast() const {
		return { std::dynamic_pointer_cast<T>(ptr.lock()) };
	}

	void Invalidate() { ptr = {}; }

private:

	std::weak_ptr<Type> ptr;
};

