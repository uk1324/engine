#pragma once

#include "Vertex2dPtData.hpp"

static constexpr Vertex2dPt quad2dPtVertices[]{
	{ Vec2(-1.0f, 1.0f), Vec2(0.0f, 1.0f) },
	{ Vec2(1.0f, 1.0f), Vec2(1.0f, 1.0f) },
	{ Vec2(-1.0f, -1.0f), Vec2(0.0f, 0.0f) },
	{ Vec2(1.0f, -1.0f), Vec2(1.0f, 0.0f) },
};
static constexpr u32 quad2dPtIndices[]{ 0, 1, 2, 2, 1, 3 };

void quad2dPtDrawInstances(usize count);
void quad2dPtDraw();