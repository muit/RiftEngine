// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Editor/Widgets/PropertyWidget.h"
#include "Reflection/Runtime/TPropertyHandle.h"


class StringPropertyWidget : public PropertyWidget {
	CLASS(StringPropertyWidget, PropertyWidget)

	std::shared_ptr<TPropertyHandle<String>> prop;

public:

	void Configure(const std::shared_ptr<TPropertyHandle<String>>& inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		StringUtils::ToSentenceCase(idName, displayName);
	}

protected:

	virtual void Tick() override;

private:
	void GetValue();
};

#endif
