// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "StructType.h"
#include <EASTL/type_traits.h>

#include "Core/Reflection/StructType.h"
#include "TProperty.h"


/** TStruct will be specialized for each type at compile time and store
 * the metadata for that type
 */
template <typename Type>
class TStruct : public StructType {
	static_assert(eastl::is_convertible< Type, Struct >::value, "Type does not inherit Pod!");
private:

	static TStruct _struct;


public:

	TStruct() : StructType() {
		Type::__meta_RegistryStruct();
		Type::__meta_RegistryProperties();
	}

public:

	/** GENERATION */

	/** Registry an struct */
	void Registry(const Name& inName)
	{
		name = inName;
	}

	/** Registry an struct with a parent */
	template<typename Super>
	void Registry(Name&& inName)
	{
		parent = Super::StaticStruct();
		parent->RegistryChild(this);
		Registry(eastl::move(inName));
	}

	/** Registry an struct's tags */
	void RegistryTags(ReflectionTags inTags)
	{
		tags = inTags;
	}

	template<typename VarType>
	void RegistryProperty(Name&& name, eastl::function<VarType*(BaseStruct*)>&& access, ReflectionTags tags)
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

template <typename Type>
TStruct<Type> TStruct<Type>::_struct {};
