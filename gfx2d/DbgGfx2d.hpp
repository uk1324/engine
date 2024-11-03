#pragma once

#include <engine/Math/Vec3.hpp>
#include <engine/Math/Vec2.hpp>
#include <vector>

namespace Dbg {

	static constexpr Vec3 DEFAULT_COLOR(1.0f);
	void disk(Vec2 pos, f32 radius, Vec3 color = DEFAULT_COLOR);
	void line(Vec2 e0, Vec2 e1, f32 width, Vec3 color = DEFAULT_COLOR);
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

#ifndef FINAL_RELEASE
	extern std::vector<Disk> disks;
	extern std::vector<Line> lines;
#endif
}