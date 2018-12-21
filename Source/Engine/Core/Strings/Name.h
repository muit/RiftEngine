// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "../Platform/Platform.h"
#include "String.h"
#include <EASTL/functional.h>
#include <EASTL/unordered_set.h>


struct Name;

/** Global table storing all names */
class NameTable {
	friend Name;
	using Container = eastl::unordered_set<String>;
	using Iterator      = Container::iterator;
	using ConstIterator = Container::const_iterator;

	Container table;


	NameTable() : table{} {}

	ConstIterator Init(const String& string);

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

	Name() : id(NoneId()) {}

	Name(const StringView&& key) : Name(String{ key }) {}
	Name(const ANSICHAR* key) : Name(String{ key }) {}
	Name(const String& key) {
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

	String ToString() const {
		if (IsNone())
		{
			return "";
		}
		return *id;
	}

	bool operator==(const Name& other) const {
		return id == other.id;
	}

	bool IsNone() const {
		return id == NoneId();
	}

	const Id& GetId() const { return id; }


	static const Name None() { return { NoneId() }; };
	static const Id NoneId() { return NameTable::GetGlobal().None(); };


private:

	Name(const Id& id) : id(id) {}
};

namespace eastl {
	template <>
	struct hash<Name> {
		size_t operator()(const Name& k) const
		{
			// Return the hash of the string
			static const eastl::hash<String>hasher{};
			const Name::Id& id = k.GetId();
			return hasher(*id);
		}
	};
}
