// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#include <vector>
#include "Object.h"


#define W(Widget, ...) __Add<Widget>(__VA_ARGS__)

class Widget : public Object {
	CLASS(Widget, Object)

	std::vector<GlobalPtr<Widget>> childs;

	bool bBuilding = true;
	bool bBuilt = true;

public:

	Widget() : Super(), bBuilding(false), bBuilt(false) {}

	/** Called after widget creation with parameters.
	 * For custom parameters create your own
	 * void Configure(...)
	 */
	virtual void Configure() {}

	/** called AFTER parent widget is built */
	void OnBuild() {
		if (!bBuilt && !bBuilding)
		{
			bBuilding = true;
			Build();
			bBuilding = false;
			bBuilt = true;

			for (auto& child : childs)
			{
				child->OnBuild();
			}
		}
	}

	void ForceRebuild() {
		if (!bBuilding)
		{
			bBuilt = false;
			UndoBuild();
			childs.clear();
			OnBuild();
		}
	}

	void OnTick() {
		if (bBuilt)
		{
			Tick();
		}
	}

	void TickChilds() {
		for (auto& child : childs)
		{
			child->OnTick();
		}
	}

protected:
	/** Create child widgets here with W() */
	virtual void Build() {}
	/** Called before rebuilding or when the widget gets destroyed */
	virtual void UndoBuild() {}

	virtual void Tick() { TickChilds(); }

	virtual void BeforeDestroy() override {
		Super::BeforeDestroy();
		UndoBuild();
	}

	template<typename WidgetType, typename... Args>
	Ptr<WidgetType> __Add(Args&&... args) {
		if (bBuilding)
		{
			// Create the widget
			GlobalPtr<WidgetType> widget = Create<WidgetType>();

			// Registry the widget
			Ptr<WidgetType> ptr = widget.GetPtr();
			childs.push_back(std::move(widget));

			// Configure it
			ptr->Configure(std::forward<Args>(args)...);
			return ptr;
		}
		return {};
	}

public:

	template<typename WidgetType, typename... Args>
	static GlobalPtr<WidgetType> CreateStandalone(Args&&... args) {
		// Create the widget
		GlobalPtr<WidgetType> widget = Create<WidgetType>();

		// Configure it
		widget->Configure(std::forward<Args>(args)...);
		widget->OnBuild();
		return std::move(widget);
	}
};
