// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Editor/Widgets/PropertyWidget.h"
#include <imgui/imgui.h>
#include "Reflection/Runtime/TPropertyHandle.h"


class FloatPropertyWidget : public PropertyWidget {
	CLASS(FloatPropertyWidget, PropertyWidget)

	std::shared_ptr<TPropertyHandle<float>> prop;

public:

	void Configure(const std::shared_ptr<TPropertyHandle<float>>& inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		StringUtils::ToSentenceCase(idName, displayName);
	}

protected:

	virtual void Tick() override;
};

#endif
