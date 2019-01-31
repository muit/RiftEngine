// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreObject.h"

#include "../Events/Broadcast.h"
#include "Keys.h"


using KeyBroadcast = Broadcast<EKey, EKeyModifier, EKeyPressState>;
using KeyPressedBroadcast = Broadcast<EKey, EKeyModifier>;
using AxisBroadcast = Broadcast<EAxis, float>;


class InputManager : public Object {
	CLASS(InputManager, Object)

private:

	/** Input State */

	struct AxisStates {
		TArray<EAxis> axis;
		TArray<float> values;
		TArray<bool> dirtyness;

		AxisBroadcast onAllAxis;
		TArray<AxisBroadcast> onAxis;

		i32 Add(EAxis a, float value)
		{
			axis.Add(a);
			values.Add(value);
			dirtyness.Add(true);
			onAxis.AddDefaulted();

			return axis.Size() - 1;
		}
	};

	struct KeyStates {
		TArray<EKey> keys;
		TArray<EKeyPressState> states;
		TArray<bool> dirtyness;

		i32 Add(EKey key)
		{
			keys.Add(key);
			states.Add(EKeyPressState::None);
			dirtyness.Add(true);

			return keys.Size() - 1;
		}
	};

	AxisStates axisStates;
	KeyStates keyStates;


	/** Allows complex flag key combinations. E.g: W | UpArrow */
	mutable TArray<TPair<EKey, KeyBroadcast>> keyBindings;
	mutable TArray<TPair<EKey, KeyPressedBroadcast>> keyPressedBindings;

	KeyBroadcast onKey;
	KeyPressedBroadcast onKeyPressed;

public:

	bool Tick(float deltaTime, Ptr<class UIManager> ui, Ptr<class Renderer> renderer);

private:

	void UpdateAxis(EAxis axis, float value);
	void UpdateKey(EKey key, EKeyPressState state);

	void NotifyAllAxis();
	void NotifyAllKeys();

public:


	/** EVENTS */

	/** Called once when any key is pressed down or up */
	const KeyBroadcast& OnKey() const { return onKey; }

	/** Called once when any of the provided keys is pressed down or up */
	const KeyBroadcast& OnKey(EKey keys) const;


	/** Called every tick if any of the provided keys is pressed */
	const KeyPressedBroadcast& OnKeyPressed() const { return onKeyPressed; }

	/** Called every tick if any of the provided keys is pressed */
	const KeyPressedBroadcast& OnKeyPressed(EKey keys) const;


	const AxisBroadcast& OnAxis() const { return axisStates.onAllAxis; }
	const AxisBroadcast& OnAxis(EAxis axis) {
		i32 i = axisStates.axis.FindIndex(axis);
		if (i == NO_INDEX)
			i = axisStates.Add(axis, 0.f);

		return axisStates.onAxis[i]; }
};
