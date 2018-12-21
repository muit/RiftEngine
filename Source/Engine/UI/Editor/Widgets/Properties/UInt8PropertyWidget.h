// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "../PropertyWidget.h"

#include <imgui/imgui.h>
#include "Core/Reflection/Runtime/TPropertyHandle.h"


class UInt8PropertyWidget : public PropertyWidget {
	CLASS(UInt8PropertyWidget, PropertyWidget)

	eastl::shared_ptr<TPropertyHandle<uint8>> prop;

public:

	void Configure(const eastl::shared_ptr<TPropertyHandle<uint8>>& inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		StringUtils::ToSentenceCase(idName, displayName);
	}

protected:

	virtual void Tick() override;
};

#endif
