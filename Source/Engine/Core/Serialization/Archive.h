// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/stack.h>
#include <nlohmann/json.hpp>

#include "EngineTypes.h"
#include "Core/TypeTraits.h"
#include "Core/Strings/Name.h"

#include "Core/Object/ObjectPtr.h"

#include "MemoryReader.h"
#include "MemoryWriter.h"

using json = nlohmann::json;


class Archive {

	const bool bReads = false;

public:

	Archive() : bReads(false) {}
	Archive(bool bReads) : bReads(bReads) {}
	virtual ~Archive() = default;


	template<typename T>
	FORCEINLINE Archive& operator()(const char* name, T& val) {
		Serialize(*this, name, val);
		return *this;
	}


	virtual void Serialize(Archive& ar, const char* name, uint8& val) = 0;

	virtual void Serialize(Archive& ar, const char* name, int32& val) = 0;

	virtual void Serialize(Archive& ar, const char* name, float& val) = 0;

	virtual void Serialize(Archive& ar, const char* name, Name& val) = 0;

	virtual void Serialize(Archive& ar, const char* name, String& val) = 0;

	template<typename T>
	void Serialize(Archive& ar, const char* name, GlobalPtr<T>& val) {
		BeginObject(name);

		if (ar.IsLoading())
		{
			// #TODO: Create Object
		}

		if (val)
		{
			val->Serialize(ar);
		}
		EndObject();
	}

	template<typename T>
	void Serialize(Archive& ar, const char* name, Ptr<T>& val) {
		if (ar.IsSaving())
		{
			ar(name, val? val->GetName() : nullptr);
		}
		else
		{
			// #TODO: Load pointer. Find object and assign it
			// ar(name, ...);
		}
	}

	template<typename T>
	void Serialize(Archive& ar, const char* name, eastl::vector<T>& val)
	{
		BeginObject(name);
		if (ar.IsLoading())
		{
			// Read Amount
			// Read each Item
		}
		else
		{
			// Save Amount
			// Save Items
		}
		EndObject();
	}

	virtual void BeginObject(const char* name) = 0;
	virtual void EndObject() = 0;

	FORCEINLINE bool IsLoading() { return bReads; }
	FORCEINLINE bool IsSaving() { return !bReads; }
};


class JsonArchive : public Archive {

	json baseData;
	eastl::stack<json*> depthData;


public:

	JsonArchive() : Archive(), baseData{}, depthData{} {}
	virtual ~JsonArchive() = default;

	String GetDataString() { return baseData.dump(); }

private:

	virtual void BeginObject(const char* name) override {
		depthData.push(&Data()[name]);
	}

	virtual void EndObject() override {
		depthData.pop();
	}


	virtual void Serialize(Archive&, const char* name, uint8& val) override {
		if (IsLoading())
			val = Data()[name].get<uint8>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(Archive&, const char* name, int32& val) override {
		if (IsLoading())
			val = Data()[name].get<int32>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(Archive&, const char* name, float& val) override {
		if (IsLoading())
			val = Data()[name].get<float>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(Archive& ar, const char* name, Name& val) override {
		String str = val.ToString();
		ar(name, str);
		if (IsLoading())
			val = str;
	}

	virtual void Serialize(Archive&, const char* name, String& val) override {
		if (IsLoading())
			val = Data()[name].get<String>();
		else
			Data()[name] = val;
	}

	json& Data() {
		if(depthData.size() > 0)
			return *depthData.top();
		return baseData;
	}
};
