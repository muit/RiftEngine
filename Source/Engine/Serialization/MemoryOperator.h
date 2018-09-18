// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "Archive.h"

class MemoryOperator {
public:

	const bool bReads = false;

protected:

	MemoryOperator(bool bReads) : bReads(bReads) {}
};
