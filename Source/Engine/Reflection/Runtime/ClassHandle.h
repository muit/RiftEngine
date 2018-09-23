// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "EngineTypes.h"
#include "Reflection/Class.h"
#include "TPropertyHandle.h"


class ClassHandle {

	Class* bindedClass;

public:

	ClassHandle() = default;
	ClassHandle(ClassHandle&&) = default;
	ClassHandle(const ClassHandle&) = default;
	ClassHandle& operator=(ClassHandle&&) = default;
	ClassHandle& operator=(const ClassHandle&) = default;
	virtual ~ClassHandle() = default;

	ClassHandle(Class* inClass) : bindedClass(inClass) {}

};
