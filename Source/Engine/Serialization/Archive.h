// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "MemoryReader.h"
#include "MemoryWriter.h"

class Archive {
	const MemoryOperator memoryOperator;

	Archive(const MemoryReader& reader) : memoryOperator(reader) {}
	Archive(const MemoryWriter& writer) : memoryOperator(writer) {}

	bool IsLoading() { return memoryOperator.bReads; }
	bool IsSaving() { return !memoryOperator.bReads; }
};
