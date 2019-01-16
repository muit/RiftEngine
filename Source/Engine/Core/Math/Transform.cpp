// Copyright 2015-2019 Piperift - All rights reserved

#include "Transform.h"
#include "Core/Serialization/Archive.h"

#if WITH_EDITOR
#include <EASTL/shared_ptr.h>
#include <imgui/imgui.h>
#include "UI/Editor/Widgets/PropertyWidget.h"

#include "Core/Reflection/Runtime/TPropertyHandle.h"
#endif


bool Transform::Serialize(Archive& ar, const char* name)
{
	ar.BeginObject(name);
	{
		ar("location", location);
		ar("rotation", rotation);
		ar("scale", scale);
	}
	ar.EndObject();
	return true;
}


void Transform::CreateDetailsWidget(Transform& val)
{

}

#if WITH_EDITOR

class TransformPropertyWidget : public PropertyWidget {
	CLASS(TransformPropertyWidget, PropertyWidget)

		eastl::shared_ptr<TPropertyHandle<Transform>> prop;

public:

	void Configure(const eastl::shared_ptr<TPropertyHandle<Transform>>& inProperty)
	{
		prop = inProperty;
		idName = prop->GetName();
		CString::ToSentenceCase(idName, displayName);
	}

protected:

	virtual void Tick() override;
};


#endif