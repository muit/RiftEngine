// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"
#include "Core/Strings/String.h"
#include "Core/Strings/Name.h"
#include "Core/Math/Vector.h"


class Scene : public Object {
	CLASS(Scene, Object)

public:

	PROP(String, displayName, DetailsEdit)
	String displayName;

	PROP(Name, someName, DetailsEdit)
	Name someName;

	PROP(i32, aNumber, DetailsEdit)
	i32 aNumber;

	PROP(u8, anotherNumber, DetailsEdit | Transient)
	u8 anotherNumber;

	PROP(v3, myVector, DetailsEdit)
	v3 myVector;

	void Tick(float /*deltaTime*/) {
	}
};
