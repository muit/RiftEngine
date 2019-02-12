// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"

#include "../Events/Broadcast.h"
#include "Keys.h"


using KeyBroadcast = Broadcast<EKey, EKeyModifier, EKeyPressState>;
using KeyPressedBroadcast = Broadcast<EKey, EKeyModifier>;
using AxisBroadcast = Broadcast<EAxis, float>;



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
	Name name;
};
