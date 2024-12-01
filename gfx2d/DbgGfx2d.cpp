#include "DbgGfx2d.hpp"

void Dbg::disk(Vec2 pos, f32 radius, Vec3 color) {
#ifndef FINAL_RELEASE
	disks.push_back(Disk{ .pos = pos, .radius = radius, .color = color });
#endif
}

void Dbg::line(Vec2 e0, Vec2 e1, f32 width, Vec3 color) {
#ifndef FINAL_RELEASE
	lines.push_back(Line{ .e0 = e0, .e1 = e1, .width = width, .color = color });
#endif
}

void Dbg::circleArc(f32 angleStart, f32 angleEnd, Vec2 center, f32 radius, f32 width, Vec3 color) {
#ifndef FINAL_RELEASE
	circleArcs.push_back(CircleArc{ .angleStart = angleStart, .angleEnd = angleEnd, .center = center, .radius = radius, .width = width, .color = color });
#endif
}

void Dbg::update() {
#ifndef FINAL_RELEASE
	disks.clear();
	lines.clear();
	circleArcs.clear();
#endif
}

#ifndef FINAL_RELEASE
std::vector<Dbg::Disk> Dbg::disks;
std::vector<Dbg::Line> Dbg::lines;
std::vector<Dbg::CircleArc> Dbg::circleArcs;
#endif 
