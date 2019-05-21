// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <assert.h>
#include <sparsehash/sparse_hash_map.h>
#include <sparsehash/dense_hash_map.h>
#include <EASTL/functional.h>

#include "CoreEngine.h"
#include "Core/Platform/Platform.h"


constexpr i32 NO_INDEX = -1;

enum EMapType {
	Smaller,
	Faster
};

template<typename Key, typename Value, EMapType type = Faster>
class TMap {
public:
	template <typename OtherType, EMapType type>
	friend class TMap;

	using KeyType = Key;
	using ValueType = Value;
	using HashMapType = google::dense_hash_map <KeyType, ValueType, eastl::hash<KeyType>, eastl::equal_to<KeyType>>;/*eastl::conditional_t<
		type == Faster,
		google::dense_hash_map <KeyType, ValueType>, // Faster hash map
		google::sparse_hash_map<KeyType, ValueType>  // Smaller hash map
	>;*/

	using Iterator      = typename HashMapType::iterator;
	using ConstIterator = typename HashMapType::const_iterator;
	using ReverseIterator = typename HashMapType::reverse_iterator;
	using ConstReverseIterator = typename HashMapType::const_reverse_iterator;


private:

	HashMapType map;

public:

	TMap() = default;

	TMap(u32 defaultSize) : map{} {
		map.
	}
	TMap(u32 defaultSize, const Type& defaultValue) : map{ defaultSize, defaultValue } {}
	TMap(std::initializer_list<Type> initList) : map{initList} {}

	TMap(TMap&& other) = default;
	TMap(const TMap& other) = default;
	TMap& operator=(TMap&& other) = default;
	TMap& operator=(const TMap& other) = default;


	i32 Add(Type&& item) {
		map.push_back(MoveTemp(item));
		return Size() - 1;
	}

	i32 Add(const Type& item) {
		map.push_back(item);
		return Size() - 1;
	}

	i32 AddUnique(const Type item) {
		const i32 foundIndex = FindIndex(item);
		if (foundIndex == NO_INDEX)
			return Add(eastl::move(item));
		return foundIndex;
	}

	i32 AddDefaulted(u32 Amount = 0) {
		map.push_back();
		return Size() - 1;
	}

	void Append(const TMap<Type>& other) {
		if (other.Size() > 0)
		{
			if (Size() <= 0)
				CopyFrom(other)
			else
				map.insert(map.end(), other.begin(), other.end());
		}
	}

	void Append(TMap<Type>&& other) {
		if (other.Size() > 0)
		{
			if (Size() <= 0)
				MoveFrom(MoveTemp(other));
			else
				map.insert(map.end(), other.begin(), other.end());
		}
	}


	void Reserve(i32 sizeNum) { map.reserve(sizeNum); }
	void Resize(i32 sizeNum) {
		map.resize(sizeNum);
	}

	void Assign(i32 sizeNum, const Type& value) {
		map.assign(sizeNum, value);
	}

	void AssignAll(const Type& value) {
		Assign(Size(), value);
	}

	void Insert(i32 index, Type&& item) {
		map.insert(map.begin() + index, eastl::move(item));
	}

	void Insert(i32 index, const Type& item, i32 count = 1) {
		if (IsValidIndex(index))
		{
			if (count == 1)
				map.insert(map.begin() + index, item);
			else
				map.insert(map.begin() + index, count, item);
		}
	}

	void InsertDefaulted(i32 index, i32 count = 1) {
		Insert(index, {}, count);
	}


	FORCEINLINE Iterator FindIt(const Type& item) const {
		return const_cast<Iterator>(eastl::find(map.begin(), map.end(), item));
	}

	FORCEINLINE Iterator FindIt(eastl::function<bool(const Type&)> cb) const {
		return const_cast<Iterator>(eastl::find_if(map.begin(), map.end(), cb));
	}

	i32 FindIndex(const Type& item) const {
		ConstIterator found = FindIt(item);
		if (found != map.end())
		{
			return (i32)eastl::distance(map.begin(), found);
		}
		return NO_INDEX;
	}

	i32 FindIndex(eastl::function<bool(const Type&)> cb) const {
		ConstIterator found = FindIt(eastl::move(cb));
		if (found != map.end())
		{
			return (i32)eastl::distance(map.begin(), found);
		}
		return NO_INDEX;
	}

	Type* Find(const Type& item) const {
		Type* const it = FindIt(item);
		return it != end() ? it : nullptr;
		return FindIt(item);
	}

	Type* Find(eastl::function<bool(const Type&)> cb) const {
		Type* const it = FindIt(eastl::move(cb));
		return it != end() ? it : nullptr;
	}

	bool Contains(const Type& item) const {
		return FindIt(item) != map.end();
	}

	bool Contains(eastl::function<bool(const Type&)> cb) const {
		return FindIt(eastl::move(cb)) != map.end();
	}

	/**
	 * Delete all items that match another provided item
	 * @return number of deleted items
	 */
	i32 Remove(const Type& item, const bool shouldShrink = true) {
		const i32 lastSize = Size();
		eastl::remove(map.begin(), map.end(), item);

		if (shouldShrink) Shrink();

		return lastSize - Size();
	}

	/**
	 * Delete item at index
	 * @return true if removed
	 */
	bool RemoveAt(i32 index, const bool shouldShrink = true) {
		if (IsValidIndex(index))
		{
			const i32 lastSize = Size();
			map.erase(map.begin() + index);

			if (shouldShrink) Shrink();

			return lastSize - Size() > 0;
		}
		return false;
	}

	/**
	 * Delete item at index. Doesn't preserve order but its considerably faster
	 * @return true if removed
	 */
	bool RemoveAtSwap(i32 index, const bool shouldShrink = true) {
		if (IsValidIndex(index))
		{
			const i32 lastSize = Size();
			Swap(index, lastSize - 1);
			map.pop_back();

			if (shouldShrink) Shrink();

			return lastSize - Size() > 0;
		}
		return false;
	}

	/**
	 * Delete all items that match a delegate
	 * @return number of deleted items
	 */
	i32 RemoveIf(eastl::function<bool(const Type&)>&& callback, const bool shouldShrink = true) {
		const i32 lastSize = Size();
		map.erase(eastl::remove_if(map.begin(), map.end(), callback), map.end());

		if (shouldShrink) Shrink();

		return lastSize - Size();
	}

	void Swap(i32 firstIndex, i32 secondIndex);

	/** Empty the array.
	 * @param shouldShrink false will not free memory
	 */
	void Empty(const bool shouldShrink = true, i32 sizeNum = 0) {
		map.clear();

		if (shouldShrink)
			Shrink();
		else if(sizeNum > 0)
			Reserve(sizeNum);
	}

	void Shrink() { map.shrink_to_fit(); }

	FORCEINLINE i32 Size() const { return (i32)map.size(); }

	FORCEINLINE bool IsValidIndex(i32 index) const
	{
		return index >= 0 && index < Size();
	}

	Type& First() { return map.front(); }
	Type& Last() { return map.back(); }
	const Type& First() const { return map.front(); }
	const Type& Last() const { return map.back(); }

	Type* Data() { return map.data(); }
	const Type* Data() const { return map.data(); }

	/** OPERATORS */
public:

	/**
	 * Array bracket operator. Returns reference to element at give index.
	 *
	 * @returns Reference to indexed element.
	 */
	FORCEINLINE Type& operator[](i32 index)
	{
		assert(IsValidIndex(index));
		return map.at(index);
	}

	/**
	 * Array bracket operator. Returns reference to element at give index.
	 *
	 * Const version of the above.
	 *
	 * @returns Reference to indexed element.
	 */
	FORCEINLINE const Type& operator[](i32 index) const
	{
		assert(IsValidIndex(index));
		return map.at(index);
	}

	FORCEINLINE Iterator      begin()        { return map.begin();  };
	FORCEINLINE ConstIterator begin()  const { return map.begin();  };
	FORCEINLINE ConstIterator cbegin() const { return map.cbegin(); };

	FORCEINLINE Iterator      end()        { return map.end();  };
	FORCEINLINE ConstIterator end()  const { return map.end();  };
	FORCEINLINE ConstIterator cend() const { return map.cend(); };

	FORCEINLINE ReverseIterator      rbegin()        { return map.rbegin();  };
	FORCEINLINE ConstReverseIterator rbegin()  const { return map.rbegin();  };
	FORCEINLINE ConstReverseIterator crbegin() const { return map.crbegin(); };

	FORCEINLINE ReverseIterator      rend()        { return map.rend();  };
	FORCEINLINE ConstReverseIterator rend()  const { return map.rend();  };
	FORCEINLINE ConstReverseIterator crend() const { return map.crend(); };


	/** INTERNAl */
private:

	FORCEINLINE void CopyFrom(const TMap& other) { map = other.map; }
	FORCEINLINE void MoveFrom(TMap&& other) { map = eastl::move(other.map); }
};


template<typename Type, typename Allocator /*= EASTLAllocatorType*/>
void TMap<Type, Allocator>::Swap(i32 firstIndex, i32 secondIndex)
{
	if (Size() > 1 && firstIndex != secondIndex && IsValidIndex(firstIndex) && IsValidIndex(secondIndex))
	{
		eastl::iter_swap(map.begin() + firstIndex, map.begin() + secondIndex);
	}
}
