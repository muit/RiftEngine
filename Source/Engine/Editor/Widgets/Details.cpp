// Copyright 2015-2019 Piperift - All rights reserved

#include "Details.h"
#include "Reflection/Runtime/HandleHelper.h"


#if WITH_EDITOR

void Details::Build()
{
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
	//if (object.IsValid())
	{
		ImGui::Begin("Details");
		{
			ImGui::Text("Name  ");
			ImGui::SameLine();
			static char str0[128] = "Object!";
			static int i0 = 123;
			ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
			ImGui::Separator();
			TickChilds();
		}
		ImGui::End();
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
