// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "../PropertyWidget.h"

#include <imgui/imgui.h>
#include "Core/Reflection/Runtime/TPropertyHandle.h"


class V3PropertyWidget : public PropertyWidget {
	CLASS(V3PropertyWidget, PropertyWidget)

	eastl::shared_ptr<TPropertyHandle<v3>> prop;

public:

	void Configure(const eastl::shared_ptr<TPropertyHandle<v3>>& inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		CString::ToSentenceCase(idName, displayName);
	}

protected:

	virtual void Tick() override;
};

#endif
