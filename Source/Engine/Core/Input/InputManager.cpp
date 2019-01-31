// Copyright 2015-2019 Piperift - All rights reserved

#include "InputManager.h"
#include "SDL_events.h"
#include "Rendering/Renderer.h"
#include "UI/UIManager.h"


bool InputManager::Tick(float deltaTime, Ptr<UIManager> ui, Ptr<Renderer> renderer)
{
	ZoneScopedNC("Input", 0x459bd1);

	// Mark all axis not dirty
	axisStates.dirtyness.Empty(false);
	axisStates.dirtyness.Resize(axisStates.axis.Size(), false);

	keyStates.dirtyness.Empty(false);
	keyStates.dirtyness.Resize(keyStates.keys.Size(), false);

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
		}

		// #TODO: Detect keys
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
		axisStates.dirtyness[i] = true;
	}
}

void InputManager::UpdateKey(EKey key, EKeyPressState state)
{

}

void InputManager::NotifyAllAxis()
{
	for (i32 i = 0; i < axisStates.axis.Size(); ++i)
	{
		if (axisStates.dirtyness[i])
		{
			const EAxis& axis = axisStates.axis[i];
			const float& finalValue = axisStates.values[i];

			axisStates.onAllAxis.DoBroadcast(axis, finalValue);
			axisStates.onAxis[i].DoBroadcast(axis, finalValue);
		}
	}
}

void InputManager::NotifyAllKeys()
{}

const KeyBroadcast& InputManager::OnKey(EKey keys) const
{
	// If binding doesn't exist, create it
	i32 id = keyBindings.FindIndex([keys](const auto& binding) {
		return binding.first == keys;
	});
	if (id == NO_INDEX) {
		id = keyBindings.Add({ keys, {} });
	}
	return keyBindings[id].second;
}

const KeyPressedBroadcast& InputManager::OnKeyPressed(EKey keys) const
{
	// If binding doesn't exist, create it
	i32 id = keyPressedBindings.FindIndex([keys](const auto& binding) {
		return binding.first == keys;
	});
	if (id == NO_INDEX) {
		id = keyPressedBindings.Add({ keys, {} });
	}
	return keyPressedBindings[id].second;
}
