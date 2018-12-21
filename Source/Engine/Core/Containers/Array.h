// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <EASTL/vector.h>
#include <EASTL/functional.h>
#include "../Platform/Platform.h"


template<typename Type>
class TArray {

	friend TArray;

	eastl::vector<Type> vector;

public:

	TArray() : vector{} {}
	TArray(TArray<Type>&& other) { operator=(eastl::move(other)); }
	TArray(const TArray<Type>& other) { operator=(other); }

	TArray<Type>& operator=(TArray<Type>&& other) {
		vector = eastl::move(other.vector);
		return *this;
	}

	TArray<Type>& operator=(const TArray<Type>& other) {
		vector = other.vector;
		return *this;
	}

	void Reserve(int32 sizeNum) { vector.reserve(sizeNum); }

	int32 Add(Type&& item) {
		vector.push_back(std::move(item));
		return Size() - 1;
	}

	int32 Add(const Type& item) {
		vector.push_back(item);
		return Size() - 1;
	}

	/**
	 * Delete all items that match another provided item
	 * @return number of deleted items
	 */
	int32 Delete(const Type& item) {
		int32 lastSize = Size();
		vector.erase(eastl::remove_if(vector.begin(), vector.end(), item), v.end());
		return lastSize - Size();
	}

	/**
	 * Delete item at index
	 * @return true if removed
	 */
	bool DeleteAt(int32 index) {
		int32 lastSize = Size();
		vector.erase(vector.begin() + index);
		return lastSize - Size() > 0;
	}

	/**
	 * Delete all items that match a delegate
	 * @return number of deleted items
	 */
	int32 DeleteWhere(eastl::function<bool(const Type&)>&& callback) {
		int32 lastSize = Size();
		vector.erase(eastl::remove_if(v.begin(), v.end(), callback), v.end());
		return lastSize - Size();
	}

	void Empty() { vector.empty(); }

	int32 Size() const { return vector.size(); }
};
