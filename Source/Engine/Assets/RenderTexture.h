// Copyright 2017/2019 - Miguel Fernández Arce

#pragma once

#include "CoreObject.h"
#include "Texture.h"


class RenderTexture : public Texture
{
	CLASS(RenderTexture, Texture)

public:

	virtual bool PostLoad() override;

	void Render();
};
