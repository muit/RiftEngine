// Copyright 2017/2018 - Miguel Fernández Arce

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"


class Model : public AssetData
{
	CLASS(Model, AssetData)

public:

    virtual bool PostLoad() override;
};
