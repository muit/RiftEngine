// Copyright 2017/2018 - Miguel Fernández Arce

#pragma once

#include "CoreObject.h"


class AssetInfo : public Object
{
	CLASS(AssetInfo, Object)

public:
	AssetInfo() : id(Name::None()) {}
	AssetInfo(Name id) : id(id) {}

protected:

	Name id;

public:

	/**
		* @returns true if this can never be pointed towards an asset
		*/
	const bool IsNull() const {
		return id.IsNone();
	}

	/**
		* @returns true if this can never be pointed towards an asset
		*/
	const bool IsValid() const {
		return false; // #TODO: Implement Is asset valid
	}

	inline const Name GetPath() const { return id; }
};
