// Copyright 2015-2019 Piperift - All rights reserved

#include "InputManager.h"
#include "SDL_events.h"
#include "Rendering/Renderer.h"
#include "UI/UIManager.h"


bool InputManager::Tick(float deltaTime, Ptr<UIManager> ui, Ptr<Renderer> renderer)
{
	ZoneScopedNC("Input", 0x459bd1);

	// Mark all axis not dirty
	axisStates.changed.Empty(false);
	axisStates.changed.Resize(axisStates.axis.Size(), false);

	UpdatePressedKeys();
	UpdatePressedMods();

	bool bFinish = false;
	SDL_PumpEvents();
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ui->OnSDLEvent(&event);

		switch (event.type)
		{
		case SDL_WINDOWEVENT:
			if (event.window.event != SDL_WINDOWEVENT_CLOSE || event.window.windowID != renderer->GetWindowId())
				break;
		case SDL_QUIT:
			bFinish = true;
			break;

		case SDL_MOUSEWHEEL:
			if (Math::Abs(event.wheel.x) > 0)
				UpdateAxis(EAxis::MouseX, (float)event.wheel.x);
			if (Math::Abs(event.wheel.y) > 0)
				UpdateAxis(EAxis::MouseY, (float)event.wheel.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				UpdateKey(EKey::MouseLeft, EKeyPressState::Press);
			if (event.button.button == SDL_BUTTON_RIGHT)
				UpdateKey(EKey::MouseRight, EKeyPressState::Press);
			if (event.button.button == SDL_BUTTON_MIDDLE)
				UpdateKey(EKey::MouseCenter, EKeyPressState::Press);
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			EKeyPressState state = (event.type == SDL_KEYDOWN) ? EKeyPressState::Press : EKeyPressState::Released;
			const SDL_Keymod sdlModState = SDL_GetModState();

			// Automatic casting to Rift keys
			const EKey key = (EKey)event.key.keysym.scancode;

			switch (key)
			{
			case EKey::LShift:
				UpdateMod(EKeyModifier::LShift, state);
				break;
			case EKey::RShift:
				UpdateMod(EKeyModifier::RShift, state);
				break;
			case EKey::LCtrl:
				UpdateMod(EKeyModifier::LCtrl, state);
				break;
			case EKey::RCtrl:
				UpdateMod(EKeyModifier::RCtrl, state);
				break;
			case EKey::LAlt:
				UpdateMod(EKeyModifier::LAlt, state);
				break;
			case EKey::RAlt:
				UpdateMod(EKeyModifier::RAlt, state);
				break;
			default:
				UpdateKey(key, state);
			};
			break;
		}
	}

	NotifyAllAxis();
	NotifyAllKeys();

	return bFinish;
}

void InputManager::UpdateAxis(EAxis axis, float value)
{
	i32 i = axisStates.axis.FindIndex(axis);
	if (i == NO_INDEX)
		axisStates.Add(axis, value);
	else
	{
		axisStates.values[i] = value;
		axisStates.changed[i] = true;
	}
}

void InputManager::UpdatePressedKeys()
{
	for (const auto& action : triggerActions) {
		for (const auto& binding : action.bindings)
		{

		}
	}


	for (i32 i = 0; i < keyStates.states.Size(); ++i)
	{
		switch (keyStates.states[i])
		{
		case EKeyPressState::Press:
			keyStates.states[i] = EKeyPressState::Pressed;
			break;

		case EKeyPressState::Released:
			keyStates.keys.RemoveAt(i, false);
			keyStates.states.RemoveAt(i, false);
			--i;
			break;
		}
	}
	keyStates.keys.Shrink();
	keyStates.states.Shrink();
}

void InputManager::UpdateKey(EKey key, EKeyPressState state)
{
	i32 i = keyStates.keys.FindIndex(key);
	if (i == NO_INDEX)
		keyStates.Add(key, state);
	else
	{
		EKeyPressState& currentState = keyStates.states[i];

		// Can only be pressed when creating the state
		if (state != EKeyPressState::Press)
		{
			currentState = state;
		}
	}
}

void InputManager::UpdatePressedMods()
{
	for (i32 i = 0; i < modStates.states.Size(); ++i)
	{
		switch (modStates.states[i])
		{
		case EKeyPressState::Press:
			modStates.states[i] = EKeyPressState::Pressed;
			break;

		case EKeyPressState::Released:
			modStates.mods.RemoveAt(i, false);
			modStates.states.RemoveAt(i, false);
			--i;
			break;
		}
	}
	modStates.mods.Shrink();
	modStates.states.Shrink();
}


void InputManager::UpdateMod(EKeyModifier mod, EKeyPressState state)
{
	i32 i = modStates.mods.FindIndex(mod);
	if (i == NO_INDEX)
		modStates.Add(mod, state);
	else
	{
		EKeyPressState& currentState = modStates.states[i];

		// Can only be pressed when creating the state
		if (state != EKeyPressState::Press)
		{
			currentState = state;
		}
	}
}

void InputManager::NotifyAllAxis()
{
	for (i32 i = 0; i < axisStates.axis.Size(); ++i)
	{
		if (axisStates.changed[i])
		{
			const EAxis& axis = axisStates.axis[i];
			const float& finalValue = axisStates.values[i];

			axisStates.onAllAxis.DoBroadcast(axis, finalValue);
			axisStates.onAxis[i].DoBroadcast(axis, finalValue);
		}
	}
}

void InputManager::NotifyAllKeys()
{
	for (i32 i = 0; i < keyStates.keys.Size(); ++i)
	{
		const EKey& key = keyStates.keys[i];
		EKeyPressState state = keyStates.states[i];

		if (state == EKeyPressState::Pressed)
		{
			onKeyPressed.DoBroadcast(key, EKeyModifier::None);

			const i32 id = FindKeyPressedActionIndex(key, EKeyModifier::None);
			if (id != NO_INDEX) {
				keyPressedActions[id].event.DoBroadcast(key, EKeyModifier::None);
			}
		}
		else
		{
			onKey.DoBroadcast(key, EKeyModifier::None, state);

			const i32 id = FindKeyActionIndex(key, EKeyModifier::None);
			if (id != NO_INDEX) {
				keyActions[id].event.DoBroadcast(key, EKeyModifier::None, state);
			}
		}
	}
}

TriggerAction& InputManager::CreateTriggerAction(Name name, TArray<TriggerAction::KeyBinding>&& bindings) const
{
	if (name.IsNone())
		name = { "Invalid" };

	TriggerAction* currentAction = FindTriggerAction(name);
	if (!currentAction)
	{
		triggerActions.Add({ name });
		currentAction = &triggerActions.Last();
	}

	currentAction->bindings = bindings;

	return *currentAction;
}

TriggerAction* InputManager::FindTriggerAction(Name name) const
{
	if (name.IsNone())
		return false;

	return triggerActions.Find([name](const TriggerAction& action) {
		return action.name == name;
	});
}

const KeyBroadcast& InputManager::OnKey(EKey key, EKeyModifier mods) const
{
	// If binding doesn't exist, create it
	i32 id = FindKeyActionIndex(key, mods);

	if (id == NO_INDEX) {
		id = keyActions.Add({ key, mods, {} });
	}
	return keyActions[id].event;
}

const KeyPressedBroadcast& InputManager::OnKeyPressed(EKey key, EKeyModifier mods) const
{
	// If binding doesn't exist, create it
	i32 id = FindKeyPressedActionIndex(key, mods);

	if (id == NO_INDEX) {
		id = keyPressedActions.Add({ key, mods, {} });
	}
	return keyPressedActions[id].event;
}

i32 InputManager::AxisStates::Add(EAxis a, float value)
{
	axis.Add(a);
	values.Add(value);
	changed.Add(true);
	onAxis.AddDefaulted();

	return axis.Size() - 1;
}

i32 InputManager::KeyStates::Add(EKey key, EKeyPressState state)
{
	keys.Add(key);
	states.Add(state);

	return keys.Size() - 1;
}

i32 InputManager::ModifierStates::Add(EKeyModifier mod, EKeyPressState state)
{
	mods.Add(mod);
	states.Add(state);

	return mods.Size() - 1;
}
