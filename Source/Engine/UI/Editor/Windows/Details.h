// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Editor/EditorWindow.h"
#include "UI/Editor/Widgets/PropertyWidget.h"


class Details : public EditorWindow {
	CLASS(Details, EditorWindow)

	Ptr<Object> object;

protected:

	virtual void Build() override;

	virtual void Tick() override;

public:

	void SetObject(Ptr<Object> inObject);
};

#endif
