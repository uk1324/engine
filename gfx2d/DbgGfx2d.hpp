#pragma once

#include <engine/Math/Vec3.hpp>
#include <engine/Math/Vec2.hpp>
#include <vector>

namespace Dbg {

	static constexpr Vec3 DEFAULT_COLOR(1.0f);
	void disk(Vec2 pos, f32 radius, Vec3 color = DEFAULT_COLOR);
	void update();
	struct Disk {
		Vec2 pos;
		f32 radius;
		Vec3 color;
	};

#ifndef FINAL_RELEASE
	extern std::vector<Disk> disks;
#endif
}