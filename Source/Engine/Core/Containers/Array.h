// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <assert.h>
#include <EASTL/vector.h>
#include <EASTL/functional.h>

#include "Core/Platform/Platform.h"


constexpr i32 NO_INDEX = -1;

template<typename Type, typename Allocator = EASTLAllocatorType>
class TArray {
public:
	template <typename OtherType, typename OtherAllocator>
	friend class TArray;

	using ItemType = Type;
	using VectorType = eastl::vector<Type, Allocator>;

	using Iterator = typename VectorType::iterator;
	typedef typename VectorType::const_iterator         ConstIterator;
	typedef typename VectorType::reverse_iterator       ReverseIterator;
	typedef typename VectorType::const_reverse_iterator ConstReverseIterator;


private:

	VectorType vector;

public:

	TArray() : vector{} {}

	TArray(u32 DefaultSize) : vector{ DefaultSize } {}
	TArray(std::initializer_list<Type> initList) : vector{initList} {}

	TArray(TArray<Type>&& other) { MoveFrom(eastl::move(other)); }
	TArray<Type>& operator=(TArray<Type>&& other) {
		MoveFrom(eastl::move(other));
		return *this;
	}

	TArray(const TArray<Type>& other) { CopyFrom(other); }
	TArray<Type>& operator=(const TArray<Type>& other) {
		CopyFrom(other);
		return *this;
	}

	void Reserve(i32 sizeNum) { vector.reserve(sizeNum); }

	void Resize(i32 sizeNum) { vector.resize(sizeNum); }
	void Resize(i32 sizeNum, const Type& value) { vector.resize(sizeNum, value); }

	i32 Add(Type&& item) {
		vector.push_back(eastl::move(item));
		return Size() - 1;
	}

	i32 Add(const Type& item) {
		vector.push_back(item);
		return Size() - 1;
	}

	i32 AddUnique(const Type item) {
		const i32 foundIndex = FindIndex(item);
		if (foundIndex == NO_INDEX)
			return Add(eastl::move(item));
		return foundIndex;
	}

	i32 AddDefaulted(u32 Amount = 0) {
		vector.push_back();
		return Size() - 1;
	}

	void Insert(i32 index, Type&& item) {
		vector.insert(vector.begin() + index, eastl::move(item));
	}

	void Insert(i32 index, const Type& item) {
		vector.insert(vector.begin() + index, item);
	}

	void InsertDefaulted(i32 index, i32 count) {
		if (IsValidIndex(index))
		{
			vector.reserve(vector.size() + count);
			for (i32 i = 0; i < count; ++i)
			{
				Insert(index + i, {});
			}
		}
	}

	FORCEINLINE Iterator FindIt(const Type& item) const {
		return const_cast<Iterator>(eastl::find(vector.begin(), vector.end(), item));
	}

	FORCEINLINE Iterator FindIt(eastl::function<bool(const Type&)> cb) const {
		return const_cast<Iterator>(eastl::find_if(vector.begin(), vector.end(), cb));
	}

	i32 FindIndex(const Type& item) const {
		ConstIterator found = FindIt(item);
		if (found != vector.end())
		{
			return (i32)eastl::distance(vector.begin(), found);
		}
		return NO_INDEX;
	}

	i32 FindIndex(eastl::function<bool(const Type&)> cb) const {
		Iterator found = FindIt(eastl::move(cb));
		if (found != vector.end())
		{
			return (i32)eastl::distance(vector.begin(), found);
		}
		return NO_INDEX;
	}

	Type* Find(const Type& item) const {
		return FindIt(item);
	}

	Type* Find(eastl::function<bool(const Type&)> cb) const {
		return FindIt(eastl::move(cb));
	}

	bool Contains(const Type& item) const {
		return FindIt(item) != vector.end();
	}

	bool Contains(eastl::function<bool(const Type&)> cb) const {
		return FindIt(eastl::move(cb)) != vector.end();
	}

	/**
	 * Delete all items that match another provided item
	 * @return number of deleted items
	 */
	i32 Remove(const Type& item) {
		const i32 lastSize = Size();
		eastl::remove(vector.begin(), vector.end(), item);
		return lastSize - Size();
	}

	/**
	 * Delete item at index
	 * @return true if removed
	 */
	bool RemoveAt(i32 index) {
		if (IsValidIndex(index))
		{
			const i32 lastSize = Size();
			vector.erase(vector.begin() + index);
			return lastSize - Size() > 0;
		}
		return false;
	}

	/**
	 * Delete item at index. Doesn't preserve order but its considerably faster
	 * @return true if removed
	 */
	bool RemoveAtSwap(i32 index) {
		if (IsValidIndex(index))
		{
			const i32 lastSize = Size();
			Swap(index, lastSize - 1)
			vector.pop_back();
			return lastSize - Size() > 0;
		}
		return false;
	}

	/**
	 * Delete all items that match a delegate
	 * @return number of deleted items
	 */
	i32 RemoveIf(eastl::function<bool(const Type&)>&& callback) {
		i32 lastSize = Size();
		vector.erase(eastl::remove_if(v.begin(), v.end(), callback), v.end());
		return lastSize - Size();
	}

	void Swap(i32 firstIndex, i32 secondIndex) {
		if (size() > 1 && firstIndex != secondIndex && IsValidIndex(firstIndex) && IsValidIndex(secondIndex))
		{
			eastl::swap(vector.begin() + firstIndex, vector.begin() + secondIndex);
		}
	}

	void Empty(const bool shouldShrink = true) {
		vector.clear();
		if(shouldShrink)
			vector.shrink_to_fit();
	}

	FORCEINLINE i32 Size() const { return (i32)vector.size(); }

	FORCEINLINE bool IsValidIndex(i32 index) const
	{
		return index >= 0 && index < Size();
	}

	Type& First() { return vector.front(); }
	Type& Last() { return vector.back(); }
	const Type& First() const { return vector.front(); }
	const Type& Last() const { return vector.back(); }

	Type* Data() { return vector.data(); }
	const Type* Data() const { return vector.data(); }

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
		return vector.at(index);
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
		return vector.at(index);
	}

	FORCEINLINE Iterator      begin()        { return vector.begin();  };
	FORCEINLINE ConstIterator begin()  const { return vector.begin();  };
	FORCEINLINE ConstIterator cbegin() const { return vector.cbegin(); };

	FORCEINLINE Iterator      end()        { return vector.end();  };
	FORCEINLINE ConstIterator end()  const { return vector.end();  };
	FORCEINLINE ConstIterator cend() const { return vector.cend(); };

	FORCEINLINE ReverseIterator      rbegin()        { return vector.rbegin();  };
	FORCEINLINE ConstReverseIterator rbegin()  const { return vector.rbegin();  };
	FORCEINLINE ConstReverseIterator crbegin() const { return vector.crbegin(); };

	FORCEINLINE ReverseIterator      rend()        { return vector.rend();  };
	FORCEINLINE ConstReverseIterator rend()  const { return vector.rend();  };
	FORCEINLINE ConstReverseIterator crend() const { return vector.crend(); };


	/** INTERNAl */
private:

	FORCEINLINE void CopyFrom(const TArray& other) { vector = other.vector; }
	FORCEINLINE void MoveFrom(TArray&& other) { vector = eastl::move(other.vector); }
};
