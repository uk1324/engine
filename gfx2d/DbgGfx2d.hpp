#pragma once

#include <engine/Math/Vec3.hpp>
#include <engine/Math/Vec2.hpp>
#include <View.hpp>
#include <vector>

namespace Dbg {

	static constexpr Vec3 DEFAULT_COLOR(1.0f);
	void disk(Vec2 pos, f32 radius, Vec3 color = DEFAULT_COLOR);
	void line(Vec2 e0, Vec2 e1, f32 width, Vec3 color = DEFAULT_COLOR);
	void polygon(View<const Vec2> vertices, f32 width, Vec3 color = DEFAULT_COLOR);
	void rectRotated(Vec2 center, Vec2 size, f32 angle, f32 width, Vec3 color = DEFAULT_COLOR);
	void filledRectRotated(Vec2 center, Vec2 size, f32 angle, Vec3 color = DEFAULT_COLOR);
	void circleArc(f32 angleStart, f32 angleEnd, Vec2 center, f32 radius, f32 width, Vec3 color = DEFAULT_COLOR);
	void update();
	struct Disk {
		Vec2 pos;
		f32 radius;
		Vec3 color;
	};
	struct Line {
		Vec2 e0;
		Vec2 e1;
		f32 width;
		Vec3 color;
	};
	// Chose to call it circle arc instead of circular arc, because it's shorter.
	struct CircleArc {
		f32 angleStart, angleEnd;
		Vec2 center;
		f32 radius;
		f32 width;
		Vec3 color;
	};
	struct FilledRectRotated {
		Vec2 center;
		Vec2 size;
		f32 angle;
		Vec3 color;
	};

#ifndef FINAL_RELEASE
	extern std::vector<Disk> disks;
	extern std::vector<Line> lines;
	extern std::vector<CircleArc> circleArcs;
	extern std::vector<FilledRectRotated> filledRectsRotated;
#endif
}