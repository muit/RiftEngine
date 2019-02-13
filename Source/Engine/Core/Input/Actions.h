// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"

#include "../Events/Broadcast.h"
#include "Keys.h"


using KeyBroadcast = Broadcast<EKeyPressState>;
using KeyPressedBroadcast = Broadcast<>;
using AxisBroadcast = Broadcast<float>;


struct TriggerAction {
	struct KeyBinding {
		EKey key;
		EKeyModifier mods;
	};

	Name name;
	TArray<KeyBinding> bindings;

	KeyBroadcast event;
	KeyPressedBroadcast pressedEvent;
};


struct AxisAction {
private:

	struct BaseBinding {
		EKeyModifier mods;
		float multiplier = 1.f;
	};

public:

	struct KeyBinding : public BaseBinding {
		EKey key;
	};

	struct AxisBinding : public BaseBinding {
		EAxis axis;
	};

	Name name;
	TArray<AxisBinding> axisBindings;
	TArray<KeyBinding> keyBindings;

	AxisBroadcast event;
};
