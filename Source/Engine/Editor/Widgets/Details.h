// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Widget.h"
#include "PropertyWidget.h"


class Details : public Widget {

	Ptr<Object> object;


protected:

	virtual void Build() override {
		W(PropertyWidget);
	}

	virtual void Tick() {
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

public:

	void SetObject(Ptr<Object> inObject) {
		if (inObject != object)
		{
			object = inObject;
			ForceRebuild();
		}
	}
};

#endif
