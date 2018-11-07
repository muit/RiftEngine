// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <nlohmann/json.hpp>

#include "Util/TypeTraits.h"

#include "MemoryReader.h"
#include "MemoryWriter.h"

using json = nlohmann::json;


class Archive {

	const bool bReads = false;

public:

	Archive() : bReads(false) {}
	Archive(bool bReads) : bReads(bReads) {}
	virtual ~Archive() = default;




	template<typename T, EnableIfPassByValue> // Select a << function based on if T is 8 byte or smaller and copyable
	Archive& operator<<(T val) {
		Serialize(*this, val);	return *this;
	}

	template<typename T, EnableIfNotPassByValue>
	Archive& operator<<(const T& val) {
		Serialize(*this, val);	return *this;
	}

	bool IsLoading() { return bReads; }
	bool IsSaving() { return !bReads; }
};

/* Serialize functions of any type can be defined globally or from custom archives
   It avoids any kind of virtual table
void Serialize(const float val) {

}*/


class JsonArchive {

	json data;


	JsonArchive() = default;

	void Serialize(const GlobalPtr<Object>& p) {
		data = json{};
	}

	void Deserialize(GlobalPtr<Object>& p) {

	}
};
