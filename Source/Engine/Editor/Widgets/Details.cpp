// Copyright 2015-2019 Piperift - All rights reserved

#include "Details.h"
#include "Reflection/Runtime/HandleHelper.h"


#if WITH_EDITOR

void Details::Build()
{
	bOpen = true;

	if (object && object->GetClass())
	{

		Class* objectClass = object->GetClass();
		for (const auto& property : objectClass->GetAllProperties())
		{
			auto handle = HandleHelper::CreatePropertyHandle(object, property.second.get());
			Add(PropertyWidget::NewPropertyWidget(handle));
		}
	}

	std::vector<Class*> children{};
	Object::StaticClass()->GetAllChildClasses(children);

	/*for (auto* child : children) {
		W(PropertyWidget, child);
	}*/
}

void Details::Tick()
{
	ImGui::Begin("Details", &bOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
	if (object.IsValid())
	{
		TickChilds();
	}
	ImGui::End();
}

void Details::SetObject(Ptr<Object> inObject)
{
	if (inObject != object)
	{
		object = inObject;
		ForceRebuild();
	}
}

#endif
