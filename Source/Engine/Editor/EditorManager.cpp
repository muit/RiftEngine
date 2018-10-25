// Copyright 2015-2019 Piperift - All rights reserved

#include "EditorManager.h"
#include "Memory/Allocator.h"


#if WITH_EDITOR

void EditorManager::Tick(float deltaTime)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Scene")) {}
			if (ImGui::MenuItem("Save Scene", "CTRL+S", false, false)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	details->OnTick();


	ImGui::Begin("Memory"); {                        // Create a window called "Hello, world!" and append into it.
		ImGui::Text("%s", Memory::GetAllocator()->GetName().ToString().c_str());
		ImGui::SameLine();
		ImGui::Text(" -- %i kb", (Memory::GetAllocator()->GetSize() / 1024));
		ImGui::Separator();
		ImGui::Text("%s", Memory::GetObjectsAllocator()->GetName().ToString().c_str());
		ImGui::SameLine();
		ImGui::Text("-- %i kb", (Memory::GetObjectsAllocator()->GetSize() / 1024));
		ImGui::Separator();
		ImGui::Text("%s", Memory::GetAssetsAllocator()->GetName().ToString().c_str());
		ImGui::SameLine();
		ImGui::Text(" -- %i kb", (Memory::GetAssetsAllocator()->GetSize() / 1024));
		ImGui::Separator();
		ImGui::Text("%s", Memory::GetFrameAllocator()->GetName().ToString().c_str());
		ImGui::SameLine();
		ImGui::Text("  -- %i kb", (Memory::GetFrameAllocator()->GetSize() / 1024));
	} ImGui::End();


	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!"); {                        // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &showDemoWindow);        // Edit bool storing our window open/close state
			ImGui::Checkbox("Another Window", &showAnotherWindow);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clearColor);  // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		} ImGui::End();
	}

	// 3. Show another simple window.
	if (showAnotherWindow)
	{
		ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			showAnotherWindow = false;
		ImGui::End();
	}
}

#endif
