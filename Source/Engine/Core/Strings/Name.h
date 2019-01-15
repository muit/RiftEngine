// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "String.h"
#include <EASTL/functional.h>
#include <EASTL/unordered_set.h>
#include "Core/TypeTraits.h"


struct Name;

/** Global table storing all names */
class NameTable {
	friend Name;
	using Container     = eastl::unordered_set<String>;
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


	static const String NoneStr;
	Id id;


public:

	Name() : id(NoneId()) {}

	Name(const StringView&& key) : Name(String{ key }) {}
	Name(const TCHAR* key) : Name(String{ key }) {}
	Name(const TCHAR* key, String::size_type size) : Name(String{ key, size }) {}
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

	const String& ToString() const {
		if (IsNone())
		{
			return NoneStr;
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

	bool Serialize(class Archive& ar, const char* name);

private:

	Name(const Id& id) : id(id) {}
};

DEFINE_CLASS_TRAITS(Name,
	HasCustomSerialize = true
);

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

	/// user defined literals
	///
	/// Converts a character array literal to a basic_string.
	///
	/// Example:
	///   Name s = "abcdef"n;
	///
	/// http://en.cppreference.com/w/cpp/String/basic_String/operator%22%22s
	///
#if EASTL_USER_LITERALS_ENABLED && EASTL_INLINE_NAMESPACES_ENABLED
	EA_DISABLE_VC_WARNING(4455) // disable warning C4455: literal suffix identifiers that do not start with an underscore are reserved
	inline namespace literals
	{
		inline namespace String_literals
		{
			inline Name   operator"" n(const TCHAR* str, size_t len) EA_NOEXCEPT { return Name{ str, String::size_type(len) }; }
		}
	}
	EA_RESTORE_VC_WARNING()  // warning: 4455
#endif
} // namespace eastl
