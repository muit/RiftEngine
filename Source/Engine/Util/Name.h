// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <ThirdParty/TSL/array_set.h>
#include <ThirdParty/tsl/array_hash.h>

#include "Platform/Platform.h"
#include "String.h"


struct Name;

/** Global table storing all names */
class NameTable {
	friend Name;
	using Iterator      = tsl::array_set<ANSICHAR>::iterator;
	using ConstIterator = tsl::array_set<ANSICHAR>::const_iterator;

	tsl::array_set<ANSICHAR> table;


	NameTable() : table{} {}

	ConstIterator Init(const ANSICHAR* string);

	ConstIterator Init(const std::basic_string<ANSICHAR>& string);

	ConstIterator None() const { return table.end(); }

	static NameTable& GetGlobal() {
		static NameTable global {};
		return global;
	}
};


/**
 * An string identified by id.
 * Searching, comparing and other operations are way cheaper, but creating (indexing) is more expensive.
 */
struct Name {
	friend NameTable;
	using Id = NameTable::ConstIterator;
private:

	Id id;


public:

	Name() : id(NoneId) {}

	Name(const ANSICHAR* key) {
		// Index this name
		id = NameTable::GetGlobal().Init(key);
	}
	Name(const std::basic_string<ANSICHAR>& key) {
		// Index this name
		id = NameTable::GetGlobal().Init(key);
	}
	Name(const Name& other) : id(other.id) {}
	Name(Name&& other) : id(std::move(other.id)) {}

	Name& operator= (const Name& other) {
		id = other.id;
		return *this;
	}
	Name& operator= (Name&& other) {
		id = std::move(other.id);
		return *this;
	}

	std::basic_string<ANSICHAR> ToString() const {
		return IsNone() ? std::basic_string<ANSICHAR>{id.key(), id.key_size()} : "None";
	}

	bool operator==(const Name& other) const {
		return id == other.id;
	}

	bool IsNone() const {
		return id == NoneId;
	}

	const Id& GetId() const { return id; }


	static const Name Name::None;

private:

	static const Id Name::NoneId;
};

namespace std {
	template <>
	struct hash<Name> {
		size_t operator()(const Name& k) const
		{
			static const tsl::ah::str_hash<ANSICHAR> hasher{};
			// Return the hash of the string
			const Name::Id& id = k.GetId();
			return hasher(id.key(), id.key_size());
		}
	};
}


/** DEFINITIONS
 * We won't a cpp to have data initialized from start
 */

NameTable::ConstIterator NameTable::Init(const ANSICHAR* string)
{
	if (std::strlen(string) == 0)
		return None();

	ConstIterator FoundIt = table.find(string);
	if (FoundIt != None())
		return FoundIt;
	else
	{
		return table.insert(string).first;
	}
}

NameTable::ConstIterator NameTable::Init(const std::basic_string<ANSICHAR>& string)
{
	if (string.size() == 0)
		return None();

	ConstIterator FoundIt = table.find(string);
	if (FoundIt != None())
		return FoundIt;
	else
	{
		return table.insert(string).first;
	}
}

const Name::Id Name::NoneId{ NameTable::GetGlobal().None() };
const Name Name::None{};
