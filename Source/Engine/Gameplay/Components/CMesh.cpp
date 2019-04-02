// Copyright 2015-2019 Piperift - All rights reserved

#include "CMesh.h"

void CMesh::PostSerialize(bool/* bLoaded*/)
{
	model.Load();
}
