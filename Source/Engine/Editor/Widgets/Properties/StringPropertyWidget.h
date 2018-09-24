// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "Editor/Widgets/PropertyWidget.h"
#include "Reflection/Runtime/TPropertyHandle.h"


class StringPropertyWidget : public PropertyWidget {
	CLASS(StringPropertyWidget, PropertyWidget)

	std::shared_ptr<TPropertyHandle<String>> prop;
	String name;

public:

	void Configure(const std::shared_ptr<TPropertyHandle<String>>& inProperty)
	{
		prop = inProperty;
		name = "Display Name";
		// FIX ME (GetName
		//name = prop->GetName();
	}

protected:

	virtual void Tick() override;

private:
	void GetValue();
};

#endif
