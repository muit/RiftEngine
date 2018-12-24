// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "../PropertyWidget.h"

#include <imgui/imgui.h>
#include "Core/Reflection/Runtime/TPropertyHandle.h"


class Int32PropertyWidget : public PropertyWidget {
	CLASS(Int32PropertyWidget, PropertyWidget)

	eastl::shared_ptr<TPropertyHandle<int32>> prop;

public:

	void Configure(const eastl::shared_ptr<TPropertyHandle<int32>>&  inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		CString::ToSentenceCase(idName, displayName);
	}

protected:

	virtual void Tick() override;
};

#endif
