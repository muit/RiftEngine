// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/stack.h>
#include <nlohmann/json.hpp>

#include "EngineTypes.h"
#include "Util/TypeTraits.h"
#include "Util/Name.h"

#include "Object/ObjectPtr.h"

#include "MemoryReader.h"
#include "MemoryWriter.h"

using json = nlohmann::json;


class Archive {

	const bool bReads = false;

public:

	Archive() : bReads(false) {}
	Archive(bool bReads) : bReads(bReads) {}
	virtual ~Archive() = default;


	/*template<typename T, EnableIfPassByValue> // Select a << function based on if T is 8 byte or smaller and copyable
	inline Archive& operator<<(Pair<const char*, const T> val) {
		Serialize(*this, val.first, val.second);
		return *this;
	}

	template<typename T, EnableIfNotPassByValue>
	FORCEINLINE Archive& operator<<(Pair<const char*, const T&> val) {
		Serialize(*this, val.first, val.second);
		return *this;
	}*/

	template<typename T>
	FORCEINLINE Archive& operator()(const char* name, T& val) {
		Serialize(*this, name, val);
		return *this;
	}


	virtual void BeginObject(const char* name) = 0;
	virtual void EndObject() = 0;

	virtual void Serialize(Archive& ar, const char* name, uint8& val) = 0;

	virtual void Serialize(Archive& ar, const char* name, int32& val) = 0;

	virtual void Serialize(Archive& ar, const char* name, float& val) = 0;

	virtual void Serialize(Archive& ar, const char* name, Name& val) = 0;

	virtual void Serialize(Archive& ar, const char* name, String& val) = 0;


	template<typename T>
	void Serialize(Archive& ar, const char* name, GlobalPtr<T>& val) {
		if (val)
		{
			BeginObject(name);
			val->Serialize(ar);
			EndObject();
		}
	}

	template<typename T>
	void Serialize(Archive& ar, const char* name, Ptr<T>& val) {
		if (val)
		{
			ar(name, val->GetName());
		}
	}

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
