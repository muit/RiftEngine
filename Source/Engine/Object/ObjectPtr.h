// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Object.h"


template<typename ObjectType>
class Ptr;

template<typename ObjectType>
class GlobalPtr
{
	static_assert(std::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

public:

	template<typename... Args>
	GlobalPtr(Args&&... args) :
		ptr { std::make_unique<ObjectType>(std::forward<Args>(args)...) }
	{}

	GlobalPtr(const GlobalPtr&) = delete;
	GlobalPtr(GlobalPtr&&) = default;
	GlobalPtr& operator=(const GlobalPtr&) = delete;
	GlobalPtr& operator=(GlobalPtr&&) = default;
	~GlobalPtr() {
		if()
	}

	operator Ptr<ObjectType>() const {
		return { *this };
	}
	Ptr<ObjectType> GetPtr() const { return { *this }; }
	Ptr<ObjectType> operator->() const {
		return {*this};
	}

	template<typename T>
	Ptr<T> Cast() const {
		return { std::dynamic_pointer_cast<T>(ptr) };
	}

	operator bool() const {
		return !ptr.expired();
	};

private:

	const std::shared_ptr<ObjectType> ptr;
};


template<typename ObjectType>
class Ptr
{
	friend GlobalPtr<ObjectType>;

private:

	Ptr() = default;
	Ptr(std::weak_ptr<ObjectType>&& ptr) : ptr(ptr) {}

public:

	template<typename ObjectType2>
	Ptr(const GlobalPtr<ObjectType2>& newPtr) : Ptr(newPtr.GetPtr())
	{}

	template<typename ObjectType2>
	Ptr(const Ptr<ObjectType2>& newPtr) : ptr(newPtr.ptr)
	{
		static_assert(std::is_convertible< ObjectType2, ObjectType >::value, "Type is not compatible!");
	}

	template<typename ObjectType2>
	Ptr(Ptr<ObjectType2>&& newPtr) : ptr(std::move(newPtr.ptr))
	{
		static_assert(std::is_convertible< ObjectType2, ObjectType >::value, "Type is not compatible!");
	}

	Ptr& operator=(const Ptr& other) {
		ptr = other.ptr;
		return *this;
	};
	Ptr& operator=(Ptr&& other) {
		ptr = std::move(other.ptr);
		return *this;
	}
	Ptr& operator=(const GlobalPtr<ObjectType>& other) {
		ptr = { other };
	};

	ObjectType* operator->() {
		return ptr.expired()? nullptr : ptr.lock().get();
	}

	operator bool() {
		return !ptr.expired();
	};

	template<typename T>
	Ptr<T> Cast() const {
		return { std::dynamic_pointer_cast<T>(ptr.lock()) };
	}

private:

	const std::weak_ptr<ObjectType> ptr;
};

