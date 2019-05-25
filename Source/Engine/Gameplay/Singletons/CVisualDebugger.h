// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreStruct.h"
#include "ECS/CSingleton.h"
#include "Core/Math/Color.h"


struct VisualDebuggerPolygon {
	TArray<v3> points;
	Color color;
};

struct VisualDebuggerLine {
	v3 start;
	v3 end;
	Color color;
};

struct VisualDebuggerFrame {
	TArray<VisualDebuggerLine> lines;
	TArray<VisualDebuggerPolygon> polygons;

	void Reset()
	{
		lines.Empty(false);
		polygons.Empty(false);
	}
};


class CVisualDebugger : public CSingleton {
	STRUCT(CVisualDebugger, CSingleton)

	using VDFramePtr = eastl::unique_ptr<VisualDebuggerFrame>;

	// Current visual data, switched every frame with another for rendering
	VDFramePtr queue;


public:

	CVisualDebugger() : Super() {
		queue = eastl::make_unique<VisualDebuggerFrame>();
	}

	void ReserveLines(u32 amount) {
		queue->lines.Reserve(queue->lines.Size() + amount);
	}

	void DrawLine(const v3& start, const v3& end, Color color) {
		queue->lines.Add({start, end, color});
	}


	void ReservePolygons(u32 amount) {
		queue->polygons.Reserve(queue->polygons.Size() + amount);
	}

	void DrawPolygon(TArray<v3>&& points, Color color) {
		queue->polygons.Add({MoveTemp(points), color});
	}

	void DrawPolygon(const TArray<v3>& points, Color color) {
		queue->polygons.Add({points, color});
	}

	/** Used ONLY to send data to render */
	FORCEINLINE VDFramePtr& GetQueue() { return queue; }
};
