// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/stack.h>
#include <nlohmann/json.hpp>

#include "Core/TypeTraits.h"
#include "Core/Object/ObjectPtr.h"

#include "MemoryReader.h"
#include "MemoryWriter.h"
#include <fstream>


using json = nlohmann::json;


template <typename T>
class HasGlobalSerialize
{
	typedef char a;
	typedef long b;

	template <typename C> static a test(decltype(&Serialize(eastl::declval<Archive&>(), eastl::declval<const char *>(), eastl::declval<C&>())));
	template <typename C> static b test(...);

public:
	enum { value = sizeof(test<T>(0)) == sizeof(char) };
};


class Archive {

	const bool bLoads = false;

public:

	Archive() : bLoads(false) {}
	Archive(bool bLoads) : bLoads(bLoads) {}
	virtual ~Archive() = default;


	template<typename T>
	FORCEINLINE Archive& operator()(const char* name, T& val) {
		CustomSerializeOrNot(*this, name, val);
		return *this;
	}

	virtual void Serialize(const char* name, uint8& val) = 0;

	virtual void Serialize(const char* name, int32& val) = 0;

	virtual void Serialize(const char* name, uint32& val) = 0;

	virtual void Serialize(const char* name, float& val) = 0;

	virtual void Serialize(const char* name, String& val) = 0;

	void Serialize(const char* name, v2& val);
	void Serialize(const char* name, v2_u32& val);
	void Serialize(const char* name, v3& val);

	template<typename T>
	void Serialize(const char* name, GlobalPtr<T>& val) {
		BeginObject(name);
		// Not yet supported. Hard and soft references need to take care of each other while serializing
		/*if (IsLoading())
		{
			val->Serialize(*this);
		}

		if (val)
		{
			val->Serialize(*this);
		}*/
		EndObject();
	}

	template<typename T>
	void Serialize(const char* name, Ptr<T>& val) {
		Name ptrName;
		if (IsSaving())
			ptrName = val ? val->GetName() : TX("None");

		Serialize(name, ptrName);

		/*if(IsLoading())
			Find and Assign object */
	}

	template<typename T>
	void Serialize(const char* name, TArray<T>& val)
	{
		BeginObject(name);
		if (IsLoading())
		{
			uint32 size;
			SerializeArraySize(size);
			val.Reserve(size);

			for (uint32 i = 0; i < size; ++i)
			{
				BeginObject(i);
				{
					operator()("val", val[i]);
					//Data() = Data()[""]; // Temporal patch to avoid named properties
				}
				EndObject();
			}
		}
		else
		{
			uint32 size = val.Size();
			SerializeArraySize(size);

			for (uint32 i = 0; i < size; ++i)
			{
				BeginObject(i);
				{
					//Data()[""] = Data(); // Temporal patch to avoid named properties
					operator()("val", val[i]);
				}
				EndObject();
			}
		}
		EndObject();
	}

	virtual void BeginObject(const char* name) = 0;
	virtual void BeginObject(uint32 index) = 0;
	virtual void EndObject() = 0;

	FORCEINLINE bool IsLoading() { return bLoads; }
	FORCEINLINE bool IsSaving() { return !bLoads; }

private:

	virtual void SerializeArraySize(uint32& Size) = 0;

	/**
	 * Selection of Serialize call.
	 */
	template<class T>
	FORCEINLINE typename eastl::enable_if_t<!ClassTraits<T>::HasCustomSerialize, bool>
		CustomSerializeOrNot(Archive& ar, const char* name, T& val)
	{
		ar.Serialize(name, val);
		return true;
	}

	template<class T>
	FORCEINLINE typename eastl::enable_if_t<ClassTraits<T>::HasCustomSerialize, bool>
		CustomSerializeOrNot(Archive& ar, const char* name, T& val)
	{
		return val.Serialize(ar, name);
	}
};


class JsonArchive : public Archive {

	json baseData;
	eastl::stack<json*> depthData;


public:

	//Save Constructor
	JsonArchive()
		: Archive(false)
		, baseData()
		, depthData{}
	{}

	// Load constructor
	JsonArchive(const json& data)
		: Archive(true)
		, baseData(data)
		, depthData{}
	{}

	virtual ~JsonArchive() = default;

	String GetDataString() const { return baseData.dump(); }
	const json& GetData() const { return baseData; }

private:

	virtual void Serialize(const char* name, uint8& val) override {
		if (IsLoading())
			val = Data()[name].get<uint8>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(const char* name, int32& val) override {
		if (IsLoading())
			val = Data()[name].get<int32>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(const char* name, uint32& val) override {
		if (IsLoading())
			val = Data()[name].get<uint32>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(const char* name, float& val) override {
		if (IsLoading())
			val = Data()[name].get<float>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(const char* name, String& val) override {
		if (IsLoading())
		{
			const auto field = Data()[name];
			if(field.is_string())
				val = field.get<String>();
		}
		else
			Data()[name] = val;
	}

	FORCEINLINE json& Data() {
		if(depthData.size() > 0)
			return *depthData.top();
		return baseData;
	}

	virtual void BeginObject(const char* name) override {
		depthData.push(&Data()[name]);
	}

	virtual void BeginObject(uint32 index) override {
		depthData.push(&Data()[index]);
	}

	virtual void EndObject() override {
		depthData.pop();
	}

	virtual void SerializeArraySize(uint32& size) override {
		if (IsLoading())
		{
			size = (uint32)Data().size();
		}
		else
		{
			json::array_t* ptr = Data().get_ptr<json::array_t*>();
			ptr->reserve((size_t)size);
		}
	};
};
