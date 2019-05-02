// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Widget.h"

#include <imgui/imgui.h>

#include "Core/Strings/String.h"
#include "Core/Reflection/Runtime/PropertyHandle.h"


class PropertyWidget : public Widget {
	CLASS(PropertyWidget, Widget)

protected:

	String idName;
	String displayName;

	eastl::shared_ptr<PropertyHandle> prop;


	virtual void Tick() override {}

public:

	void Configure(const eastl::shared_ptr<PropertyHandle>& inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		CString::ToSentenceCase(idName, displayName);
	}

	static GlobalPtr<PropertyWidget> NewPropertyWidget(const eastl::shared_ptr<PropertyHandle>& prop);
};

#endif
