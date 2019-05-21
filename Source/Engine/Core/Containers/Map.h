// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <assert.h>
#include <sparsehash/sparse_hash_map.h>
#include <sparsehash/dense_hash_map.h>
#include <EASTL/functional.h>

#include "CoreEngine.h"
#include "Core/Platform/Platform.h"
#include "Tuples.h"


enum EMapOptimization {
	Smaller,
	Faster
};

template<typename Key, typename Value, EMapOptimization optimization = Faster>
class TMap {
public:
	template <typename OtherKey, typename OtherValue, EMapOptimization optimization>
	friend class TMap;

	using KeyType = Key;
	using ValueType = Value;
	using HashMapType = google::dense_hash_map<KeyType, ValueType, eastl::hash<KeyType>, eastl::equal_to<KeyType>>;/*eastl::conditional_t<
		type == Faster,
		google::dense_hash_map <KeyType, ValueType, eastl::hash<KeyType>, eastl::equal_to<KeyType>>, // Faster hash map
		google::sparse_hash_map<KeyType, ValueType, eastl::hash<KeyType>, eastl::equal_to<KeyType>>  // Smaller hash map
	>;*/

	using Iterator           = typename HashMapType::iterator;
	using ConstIterator      = typename HashMapType::const_iterator;


private:

	HashMapType map;


public:

	TMap() = default;

	TMap(u32 defaultSize) : map{defaultSize} {}
	TMap(std::initializer_list<TPair<KeyType, ValueType>> initList) : map{initList} {}

	TMap(TMap&& other) = default;
	TMap(const TMap& other) = default;
	TMap& operator=(TMap&& other) = default;
	TMap& operator=(const TMap& other) = default;


	ValueType& Add(KeyType&& key, ValueType&& value) {
		return *map.insert(MoveTemp(key), MoveTemp(value));
	}

	ValueType& Add(const KeyType& key, ValueType&& value) {
		return *map.insert(key, MoveTemp(value));
	}

	ValueType& Add(KeyType&& key, const ValueType& value) {
		return *map.insert(MoveTemp(key), value);
	}

	ValueType& Add(const KeyType& key, const ValueType& value) {
		return *map.insert(key, value);
	}

	ValueType& Add(const TPair<KeyType, ValueType>& pair) {
		return *map.insert(pair.first, pair.second);
	}

	ValueType& Add(TPair<KeyType, ValueType>&& pair) {
		return *map.insert(MoveTemp(pair.first), MoveTemp(pair.second));
	}

	template<EMapOptimization otherOpt>
	void Append(const TMap<KeyType, ValueType, otherOpt>& other) {
		if (other.Size() > 0)
		{
			if (Size() <= 0)
			{
				CopyFrom(other)
			}
			else
			{
				map.insert(map.end(), other.begin(), other.end());
			}
		}
	}

	template<EMapOptimization otherOpt>
	void Append(TMap<KeyType, ValueType, otherOpt>&& other) {
		if (other.Size() > 0)
		{
			if (Size() <= 0)
			{
				MoveFrom(MoveTemp(other));
			}
			else
			{
				map.insert(map.end(), other.begin(), other.end());
			}
		}
	}

	void Reserve(i32 sizeNum) { map.reserve(sizeNum); }
	void Resize (i32 sizeNum) { map.resize(sizeNum); }

	FORCEINLINE Iterator FindIt(const KeyType& item) {
		return map.find(item);
	}

	FORCEINLINE ConstIterator FindIt(const KeyType& item) const {
		return map.find(item);
	}

	FORCEINLINE ValueType* Find(const KeyType& key) {
		Type* const it = FindIt(key);
		return it != end() ? it : nullptr;
	}

	FORCEINLINE const ValueType* Find(const KeyType& key) const {
		const Type* const it = FindIt(key);
		return it != end() ? it : nullptr;
	}

	FORCEINLINE ValueType& FindRef(const KeyType& key) const {
		ValueType* const val = Find(key);
		assert(val && "Key not found, can't dereference its value");
		return *val;
	}

	FORCEINLINE ValueType& FindRef(KeyType&& key) const {
		ValueType* const val = Find(MoveTemp(key));
		assert(val && "Key not found, can't dereference its value");
		return *val;
	}

	bool Contains(const KeyType& key) const {
		return FindIt(key) != map.end();
	}

	/**
	 * Delete all items that match another provided item
	 * @return number of deleted items
	 */
	i32 Remove(const KeyType& key, const bool shouldShrink = true)
	{
		ConstIterator it = FindIt(key);
		if (it != end())
		{
			const i32 lastSize = Size();
			map.erase(it);

			if (shouldShrink)
			{
				Shrink();
			}
			return lastSize - Size();
		}
		return 0;
	}

	/** Empty the array.
	 * @param bShouldShrink false will not free memory
	 */
	void Empty(const bool bShouldShrink = true, i32 sizeNum = 0) {

		if(bShouldShrink)
		{
			map.clear();
		}
		else
		{
			map.clear_no_resize();
			if (sizeNum > 0 && map.max_size() != sizeNum)
			{
				map.resize(sizeNum);
			}
		}
	}

	void Shrink() { map.shrink_to_fit(); }

	FORCEINLINE i32 Size() const { return (i32)map.size(); }

	FORCEINLINE bool IsValidIndex(i32 index) const
	{
		return index >= 0 && index < Size();
	}


	/** OPERATORS */
public:

	/**
	 * Array bracket operator. Returns reference to value at given key.
	 *
	 * @returns Reference to indexed element.
	 */
	FORCEINLINE ValueType& operator[](const KeyType& key) { return FindRef(key); }
	FORCEINLINE const ValueType& operator[](const KeyType& key) const {
		return FindRef(key);
	}

	/**
	 * Array bracket operator. Returns reference to value at given key.
	 *
	 * Const version of the above.
	 *
	 * @returns Reference to indexed element.
	 */
	FORCEINLINE const ValueType& operator[](i32 index) const
	{
		assert(IsValidIndex(index));
		return map.at(index);
	}

	// Iterator functions
	FORCEINLINE Iterator      begin()        { return map.begin();  };
	FORCEINLINE ConstIterator begin()  const { return map.begin();  };
	FORCEINLINE ConstIterator cbegin() const { return map.cbegin(); };

	FORCEINLINE Iterator      end()        { return map.end();  };
	FORCEINLINE ConstIterator end()  const { return map.end();  };
	FORCEINLINE ConstIterator cend() const { return map.cend(); };


	/** INTERNAl */
private:

	template<EMapOptimization otherOpt>
	FORCEINLINE void CopyFrom(const TMap<KeyType, ValueType, otherOpt>& other) {
		map = other.map;
	}

	template<EMapOptimization otherOpt>
	FORCEINLINE void MoveFrom(TMap<KeyType, ValueType, otherOpt>&& other) {
		map = eastl::move(other.map);
	}
};
