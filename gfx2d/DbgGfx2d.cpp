#include "DbgGfx2d.hpp"
#include <engine/Math/Rotation.hpp>

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

void Dbg::polygon(View<const Vec2> vertices, f32 width, Vec3 color) {
#ifndef FINAL_RELEASE
	if (vertices.size() <= 1) {
		return;
	}

	i32 previous = i32(vertices.size()) - 1;
	for (i32 i = 0; i < vertices.size(); i++) {
		line(vertices[previous], vertices[i], width, color);
		previous = i;
	}
#endif
}

void Dbg::rectRotated(Vec2 center, Vec2 size, f32 angle, f32 width, Vec3 color) {
#ifndef FINAL_RELEASE
	const Rotation rotation(angle);
	const auto halfSize = size / 2.0f;
	const Vec2 vertices[]{
		center + rotation * halfSize,
		center + rotation * Vec2(-halfSize.x, halfSize.y),
		center - rotation * halfSize,
		center + rotation * Vec2(halfSize.x, -halfSize.y),
	};
	polygon(constView(vertices), width, color);
#endif
}

void Dbg::filledRectRotated(Vec2 center, Vec2 size, f32 angle, Vec3 color) {
#ifndef FINAL_RELEASE
	filledRectsRotated.push_back(FilledRectRotated{ 
		.center = center,
		.size = size,
		.angle = angle,
		.color = color
	});
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
	filledRectsRotated.clear();
#endif
}

#ifndef FINAL_RELEASE
std::vector<Dbg::Disk> Dbg::disks;
std::vector<Dbg::Line> Dbg::lines;
std::vector<Dbg::CircleArc> Dbg::circleArcs;
std::vector<Dbg::FilledRectRotated> Dbg::filledRectsRotated;
#endif 
