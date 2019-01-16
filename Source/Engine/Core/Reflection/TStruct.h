// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Struct.h"
#include <EASTL/type_traits.h>

#include "Core/Object/Pod.h"
#include "TProperty.h"


/** TStruct will be specialized for each type at compile time and store
 * the metadata for that type
 */
template <typename StructType>
class TStruct : public Struct {
	static_assert(eastl::is_convertible< StructType, Pod >::value, "Type does not inherit Pod!");
private:

	static TStruct _struct;


public:

	TStruct() : Struct() {
		StructType::__meta_RegistryStruct();
		StructType::__meta_RegistryProperties();
	}

public:

	/** GENERATION */

	/** Registry an struct */
	void Registry(const Name& inName)
	{
		name = inName;
	}

	/** Registry an struct's tags */
	void RegistryTags(ReflectionTags inTags)
	{
		tags = inTags;
	}

	template<typename VarType>
	void RegistryProperty(Name&& name, eastl::function<VarType*(BaseObject*)>&& access, ReflectionTags tags)
	{
		properties.emplace(name, eastl::unique_ptr<Property>(
			new TProperty<VarType>(
				this,
				GetReflectableName<VarType>(),
				eastl::move(name),
				eastl::move(access),
				tags
			)
		));
	}

	static TStruct* GetStatic() { return &_struct; }
};

template <typename T>
TStruct<T> TStruct<T>::_struct {};
