// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "../PropertyWidget.h"

#include <imgui/imgui.h>
#include "Core/Reflection/Runtime/TPropertyHandle.h"


class UInt8PropertyWidget : public PropertyWidget {
	CLASS(UInt8PropertyWidget, PropertyWidget)

protected:

	virtual void Tick() override;

	TPropertyHandle<u8>* GetHandle() const {
		return dynamic_cast<TPropertyHandle<u8>*>(prop.get());
	}
};

#endif
