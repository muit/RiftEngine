// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"

#include "../Events/Broadcast.h"
#include "Keys.h"
#include "Actions.h"


using KeyBroadcast = Broadcast<EKey, EKeyModifier, EKeyPressState>;
using KeyPressedBroadcast = Broadcast<EKey, EKeyModifier>;
using AxisBroadcast = Broadcast<EAxis, float>;


class InputManager : public Object {
	CLASS(InputManager, Object)

private:

	/** Input State */

	/** Keeps the state of an axis */
	struct AxisStates {
		TArray<EAxis> axis;
		TArray<float> values;
		TArray<bool> changed;

		AxisBroadcast onAllAxis;
		TArray<AxisBroadcast> onAxis;

		i32 Add(EAxis a, float value);
	};

	/** Keeps the state of a key */
	struct KeyStates {
		TArray<EKey> keys;
		TArray<EKeyPressState> states;

		i32 Add(EKey key, EKeyPressState state);
	};

	/** Keeps the state of a modifier. TODO: Calculate mods flag */
	struct ModifierStates {
		TArray<EKeyModifier> mods;
		TArray<EKeyPressState> states;

		i32 Add(EKeyModifier mod, EKeyPressState state);
	};

	AxisStates axisStates;
	KeyStates keyStates;
	ModifierStates modStates;

	/** Actions allow complex flag key combinations. E.g: W | UpArrow */
	mutable TArray<TriggerAction> triggerActions;
	mutable TArray<AxisAction>    axisActions;

	KeyBroadcast onKey;
	KeyPressedBroadcast onKeyPressed;

public:

	bool Tick(float deltaTime, Ptr<class UIManager> ui, Ptr<class Renderer> renderer);

private:

	void UpdateAxis(EAxis axis, float value);

	void UpdatePressedKeys();
	void UpdateKey(EKey key, EKeyPressState state);

	void UpdatePressedMods();
	void UpdateMod(EKeyModifier mod, EKeyPressState state);

	void NotifyAllAxis();
	void NotifyAllKeys();

public:


	/** EVENTS */

	TriggerAction& CreateTriggerAction(Name name, TArray<TriggerAction::KeyBinding>&& bindings) const;

	TriggerAction* FindTriggerAction(Name name) const;

	TriggerAction& GetTriggerAction(Name name) const { return *FindTriggerAction(name); }

	/** Called once when any key is pressed down or up */
	const KeyBroadcast& OnKey() const { return onKey; }

	/** Called once when any of the provided keys is pressed down or up */
	const KeyBroadcast& OnKey(EKey key, EKeyModifier mods = EKeyModifier::None) const;


	/** Called every tick if any of the provided keys is pressed */
	const KeyPressedBroadcast& OnKeyPressed() const { return onKeyPressed; }

	/** Called every tick if any of the provided keys is pressed */
	const KeyPressedBroadcast& OnKeyPressed(EKey key, EKeyModifier mods = EKeyModifier::None) const;


	const AxisBroadcast& OnAxis() const { return axisStates.onAllAxis; }
	const AxisBroadcast& OnAxis(EAxis axis) {
		i32 i = axisStates.axis.FindIndex(axis);
		if (i == NO_INDEX)
			i = axisStates.Add(axis, 0.f);

		return axisStates.onAxis[i];
	}


private:

	i32 FindKeyActionIndex(EKey key, EKeyModifier) const {
		return keyActions.FindIndex([key](const auto& action) {
			return action.key == key;
		});
	}

	i32 FindKeyPressedActionIndex(EKey key, EKeyModifier) const {
		return keyPressedActions.FindIndex([key](const auto& action) {
			return action.key == key;
		});
	}
};
