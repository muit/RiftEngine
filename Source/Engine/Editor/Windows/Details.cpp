// Copyright 2015-2019 Piperift - All rights reserved

#include "Details.h"
#include "Reflection/Runtime/HandleHelper.h"


#if WITH_EDITOR

void Details::Build()
{
	Super::Build();
	windowName = "Details";
	windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;

	if (object && object->GetClass())
	{
		windowName = String("Details: ").append(object->GetName().ToString());

		Class* objectClass = object->GetClass();
		for (const auto& property : objectClass->GetAllProperties())
		{
			auto handle = HandleHelper::CreatePropertyHandle(object, property.second.get());
			Add(PropertyWidget::NewPropertyWidget(handle));
		}
	}

	eastl::vector<Class*> children{};
	Object::StaticClass()->GetAllChildClasses(children);

	/*for (auto* child : children) {
		W(PropertyWidget, child);
	}*/
}

void Details::Tick()
{
	if (bOpen)
	{
		BeginWindow();
		if (object.IsValid())
		{
			TickChilds();
		}
		EndWindow();
	}
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
