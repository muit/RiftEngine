// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "EngineTypes.h"
#include "EventHandle.h"


class BaseBroadcast {
protected:
	BaseBroadcast() = default;
};


template<typename... Params>
class Broadcast : public BaseBroadcast {

	using Method = void(Params...);
	using Function = eastl::function<void(Params...)>;

	using MethodPtr = void(*)(Params...);
	template<typename Type>
	using MemberMethodPtr = void(Type::*)(Params...);


	struct BaseListener {
		u64 id;
		Function method;
	};
	struct RawListener : public BaseListener {
		void* instance;
	};
	struct ObjListener : public BaseListener {
		Ptr<Object> object;
	};


	TArray<RawListener> rawListeners;
	TArray<ObjListener> objListeners;


public:

	Broadcast() = default;

	/** Broadcast to all binded functions */
	void DoBroadcast(const Params&... params) {
		for (RawListener& listener : rawListeners)
		{
			listener.method(params...);
		}

		for (auto& listener : objListeners)
		{
			if (listener.object)
			{
				listener.method(params...);
			}
		}
	}

	// #TODO: Scoped binding

	/** Binds an static function. Must be unbinded manually. */
	EventHandle Bind(Function method) {
		if (method)
		{
			EventHandle handle {};
			rawListeners.Add({ handle.Id(), eastl::move(method), nullptr });
			return handle;
		}
		return EventHandle::Invalid();
	}

	/** Binds an static function. Must be unbinded manually. */
	EventHandle Bind(MethodPtr method) {
		if (method)
		{
			EventHandle handle{};
			rawListeners.Add({ handle.Id(), eastl::move(method), nullptr });
			return handle;
		}
		return EventHandle::Invalid();
	}

	/** Binds a member function. Must be unbinded manually (unless its an Object). */
	template<typename Type>
	EventHandle Bind(Type* instance, MemberMethodPtr<Type> method) {
		if (instance && method)
		{
			if constexpr (IsObject<Type>::value)
			{
				return Bind(instance->GetSelf(), method);
			}
			else
			{
				EventHandle handle {};
				rawListeners.Add({ handle.Id(), std::bind(method, instance), instance });
				return handle;
			}
		}
		return EventHandle::Invalid();
	}

	/** Binds an object's function. Gets unbinded when the objects is destroyed */
	template<typename Type>
	EventHandle Bind(Ptr<Type> object, MemberMethodPtr<Type> method) {
		if (object && method)
		{
			EventHandle handle{};
			objListeners.Add({ handle.Id(), std::bind(method, *object), object });
			return handle;
		}
		return EventHandle::Invalid();
	}

	bool Unbind(const EventHandle& handle)
	{
		if (!handle) return false;

		return rawListeners.RemoveIf([handle](const auto& listener) {
			return listener.id == handle.Id();
		}) > 0;
	}

	bool UnbindAll(Ptr<Object> object) {
		if (object)
		{
			return objListeners.RemoveIf([object](const auto& listener) {
				return !listener.object || listener.object == object;
			}) > 0;
		}
		return false;
	}

	template<typename Type>
	bool UnbindAll(Type* instance) {
		if (instance)
		{
			if constexpr (IsObject<Type>::value)
			{
				return UnbindAll(instance->GetSelf());
			}
			else
			{
				return rawListeners.RemoveIf([instance](const auto& listener) {
					return listener.instance == instance;
				}) > 0;
			}
		}
		return false;
	}
};
