// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EASTL/utility.h"

#include "Core/TypeTraits.h"
#include "Core/Platform/Platform.h"
#include "Core/Containers/Array.h"
#include "Core/Strings/String.h"
#include "Core/Strings/Name.h"
#include "Core/Misc/Guid.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Transform.h"
#include "Core/Assets/AssetInfo.h"
#include "Core/Serialization/Json.h"


template<typename T>
inline Name GetReflectableName();

#define DECLARE_REFLECTION_TYPE(Type)\
template<>\
inline constexpr bool IsReflectableType<Type>() { return true; }\
template<>\
inline Name GetReflectableName<Type>() { return { TX(#Type) }; }


/** Registry new editor-supported types here and on "EngineTypes.cpp" */

/** To registry a new native type:
 * 1. Add Definition below
 * 2. Registry on HandleHelper.cpp
 * 3. Registry widget creation on PropertyWidget.cpp
 */

DECLARE_REFLECTION_TYPE(bool);
DECLARE_REFLECTION_TYPE(u8);
DECLARE_REFLECTION_TYPE(i32);
DECLARE_REFLECTION_TYPE(u32);
DECLARE_REFLECTION_TYPE(float);

DECLARE_REFLECTION_TYPE(Name);
DECLARE_REFLECTION_TYPE(String);
DECLARE_REFLECTION_TYPE(Guid);
DECLARE_REFLECTION_TYPE(Color);

DECLARE_REFLECTION_TYPE(v2);
DECLARE_REFLECTION_TYPE(v3);
DECLARE_REFLECTION_TYPE(v2_u32);
DECLARE_REFLECTION_TYPE(Quat);
DECLARE_REFLECTION_TYPE(Transform);
DECLARE_REFLECTION_TYPE(AssetInfo);

DECLARE_REFLECTION_TYPE(Json);

template <typename T1, typename T2>
using TPair = eastl::pair<T1, T2>;


template<typename T>
inline Name GetReflectableName()
{
	if constexpr (IsArrayType<T>())
	{
		if constexpr (IsReflectableType<typename T::ItemType>())
		{
			// TArray<Itemtype> name
			return { CString::Printf(TX("TArray<%s>"), GetReflectableName<typename T::ItemType>().ToString().c_str()) };
		}
		return TX("TArray<Invalid>");
	}
	else if constexpr (IsAssetType<T>())
	{
		// TAssetPtr<Itemtype> name
		return { CString::Printf(TX("TAssetPtr<%s>"), GetReflectableName<typename T::ItemType>().ToString().c_str()) };
	}

	return TX("Invalid");
}
