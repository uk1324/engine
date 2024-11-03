#include "DbgGfx2d.hpp"

void Dbg::disk(Vec2 pos, f32 radius, Vec3 color) {
	disks.push_back(Disk{ .pos = pos, .radius = radius, .color = color });
}

void Dbg::line(Vec2 e0, Vec2 e1, f32 width, Vec3 color) {
	lines.push_back(Line{ .e0 = e0, .e1 = e1, .width = width, .color = color });
}

void Dbg::update() {
	disks.clear();
	lines.clear();
}

#ifndef FINAL_RELEASE
std::vector<Dbg::Disk> Dbg::disks;
std::vector<Dbg::Line> Dbg::lines;
#endif 
