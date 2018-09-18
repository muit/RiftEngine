// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <ThirdParty/TSL/array_set.h>

#include "Platform/Platform.h"
#include "String.h"


struct Name;

class NameTable {
	friend Name;
	using Iterator = tsl::array_set<ANSICHAR>::iterator;
	using ConstIterator = tsl::array_set<ANSICHAR>::const_iterator;

	tsl::array_set<ANSICHAR> table;


	NameTable() : table{} {}

	ConstIterator Init(const ANSICHAR* string);

	ConstIterator Init(const std::basic_string<ANSICHAR>& string);

	ConstIterator None() const { return table.end(); }

	static NameTable global;
};


/**
 * An string identified by id.
 * Searching, comparing and other operations are way cheaper, but creating (indexing) is more expensive.
 */
struct Name {
	friend NameTable;

private:

	NameTable::ConstIterator id;


public:

	Name() : id(NoneId) {}

	Name(const ANSICHAR* key) {
		// Index this name
		id = NameTable::global.Init(key);
	}
	Name(const std::basic_string<ANSICHAR>& key) {
		// Index this name
		id = NameTable::global.Init(key);
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
		return IsNone() ? String{id.key(), id.key_size()} : "None";
	}

	bool operator==(const Name& other) const {
		return id == other.id;
	}

	bool IsNone() const {
		return id == NoneId;
	}


	static const Name Name::None;

private:

	static const NameTable::ConstIterator Name::NoneId;
};
