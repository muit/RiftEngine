// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "../PropertyWidget.h"

#include "Core/Reflection/Runtime/TPropertyHandle.h"


class StringPropertyWidget : public PropertyWidget {
	CLASS(StringPropertyWidget, PropertyWidget)

	eastl::shared_ptr<TPropertyHandle<String>> prop;

public:

	void Configure(const eastl::shared_ptr<TPropertyHandle<String>>& inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		CString::ToSentenceCase(idName, displayName);
	}

protected:

	virtual void Tick() override;
};

#endif
