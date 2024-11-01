#include "DbgGfx2d.hpp"

void Dbg::disk(Vec2 pos, f32 radius, Vec3 color) {
	disks.push_back(Disk{ .pos = pos, .radius = radius, .color = color });
}

void Dbg::update() {
	disks.clear();
}

#ifndef FINAL_RELEASE
std::vector<Dbg::Disk> Dbg::disks;
#endif 
