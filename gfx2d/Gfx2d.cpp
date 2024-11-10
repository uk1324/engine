#include "Gfx2d.hpp"
#include "Quad2dPt.hpp"
#include "StructUtils.hpp"
#include <engine/Math/Interpolation.hpp>
#include <gfx/ShaderManager.hpp>
#include <gfx/Instancing.hpp>
#include <engine/Math/Constants.hpp>
#include <engine/Math/Rotation.hpp>
#include <engine/Window.hpp>

Gfx2d Gfx2d::make() {
	auto instancesVbo = Vbo(1024ull * 10);

	Vbo quad2dPtVbo(quad2dPtVertices, sizeof(quad2dPtVertices));
	Ibo quad2dPtIbo(quad2dPtIndices, sizeof(quad2dPtIndices));

	auto circleVao = createInstancingVao<CircleShader>(quad2dPtVbo, quad2dPtIbo, instancesVbo);

	auto diskVao = createInstancingVao<DiskShader>(quad2dPtVbo, quad2dPtIbo, instancesVbo);

	auto lineVao = createInstancingVao<LineShader>(quad2dPtVbo, quad2dPtIbo, instancesVbo);

	auto filledTrianglesVbo = Vbo::generate();
	auto filledTrianglesIbo = Ibo::generate();
	auto filledTrianglesVao = createInstancingVao<FilledTriangleShader>(filledTrianglesVbo, filledTrianglesIbo, instancesVbo);

	auto fontRenderer = FontRenderer(quad2dPtVbo, quad2dPtIbo, instancesVbo);

	return Gfx2d{
		MOVE(quad2dPtVbo),
		MOVE(quad2dPtIbo),
		MOVE(circleVao),
		.circleShader = MAKE_GENERATED_SHADER(CIRCLE),
		.circleInstances = List<CircleInstance>::empty(),
		MOVE(diskVao),
		.diskShader = MAKE_GENERATED_SHADER(DISK),
		.diskInstances = List<DiskInstance>::empty(),
		MOVE(lineVao),
		.lineShader = MAKE_GENERATED_SHADER(LINE),
		.lineInstances = List<LineInstance>::empty(),
		.filledTrianglesIndices = List<u32>::empty(),
		.filledTrianglesVertices = List<Vertex2Pc>::empty(),
		MOVE(filledTrianglesVbo),
		MOVE(filledTrianglesIbo),
		MOVE(filledTrianglesVao),
		.filledTriangleShader = MAKE_GENERATED_SHADER(FILLED_TRIANGLE),
		MOVE(fontRenderer),
		MOVE(instancesVbo),
	};
}

i32 Gfx2d::addFilledTriangleVertex(Vec2 pos, Vec4 color) {
	const auto index = filledTrianglesVertices.size();
	filledTrianglesVertices.add(Vertex2Pc{ .position = pos, .color = color });
	return index;
}

void Gfx2d::addFilledTriangle(u32 i0, u32 i1, u32 i2) {
	filledTrianglesIndices.add(i0);
	filledTrianglesIndices.add(i1);
	filledTrianglesIndices.add(i2);
}

void Gfx2d::addFilledQuad(u32 i0, u32 i1, u32 i2, u32 i3) {
	/*
	i0---i1
	|  / |
	i3---i2
	*/
	addFilledTriangle(i0, i3, i1);
	addFilledTriangle(i2, i1, i3);
}

void Gfx2d::circle(Vec2 pos, f32 radius, f32 width, Vec3 color) {
	const auto pixelSize = getQuadPixelSizeY(radius);
	circleInstances.add(CircleInstance{
		.transform = camera.makeTransform(pos, 0.0f, Vec2(radius)),
		.color = Vec4(color, 1.0f),
		.smoothing = 3.0f / pixelSize,
		.width = width / radius
	});
}

void Gfx2d::circleTriangulated(Vec2 pos, f32 radius, f32 width, Vec3 color, i32 vertices) {
	auto addVertex = [&](Vec2 pos) -> u32 {
		const auto index = filledTrianglesVertices.size();
		filledTrianglesVertices.add(Vertex2Pc{ .position = pos, .color = Vec4(color, 1.0f) });
		return u32(index);
	};

	// TODO: Maybe clamp?
	const auto insideRadius = radius - width / 2.0f;
	const auto outsideRadius = radius + width / 2.0f;

	const auto center = addVertex(pos);
	const auto firstVertexInside = addVertex(pos + Vec2(insideRadius, 0.0f));
	const auto firstVertexOutside = addVertex(pos + Vec2(outsideRadius, 0.0f));
	u32 oldVertexInside = firstVertexInside;
	u32 oldVertexOutside = firstVertexOutside;
	for (i32 i = 1; i < vertices - 1; i++) {
		const auto t = f32(i) / f32(vertices - 1);
		const auto angle = lerp(0.0f, TAU<f32>, t);
		const auto direction = Vec2(cos(angle), sin(angle));
		const auto vertexInside = addVertex(pos + direction * insideRadius);
		const auto vertexOutside = addVertex(pos + direction * outsideRadius);

		addFilledQuad(oldVertexInside, oldVertexOutside, vertexOutside, vertexInside);

		oldVertexInside = vertexInside;
		oldVertexOutside = vertexOutside;
	}
	addFilledQuad(oldVertexInside, oldVertexOutside, firstVertexOutside, firstVertexInside);
}

void Gfx2d::circleTriangulated(Vec2 pos, f32 radius, f32 width, Vec3 color) {
	circleTriangulated(pos, radius, width, color, calculateCircleVertexCount(radius));
}

void Gfx2d::circleArcTriangulated(Vec2 pos, f32 radius, f32 startAngle, f32 endAngle, f32 width, Vec4 color, i32 vertices) {
	auto addVertex = [&](Vec2 pos) -> u32 {
		const auto index = filledTrianglesVertices.size();
		filledTrianglesVertices.add(Vertex2Pc{ .position = pos, .color = color });
		return u32(index);
	};

	// TODO: Maybe clamp?
	const auto insideRadius = radius - width / 2.0f;
	const auto outsideRadius = radius + width / 2.0f;

	const auto center = addVertex(pos);

	const auto direction = Vec2::oriented(startAngle);
	const auto firstVertexInside = addVertex(pos + direction * insideRadius);
	const auto firstVertexOutside = addVertex(pos + direction * outsideRadius);
	u32 oldVertexInside = firstVertexInside;
	u32 oldVertexOutside = firstVertexOutside;
	for (i32 i = 1; i < vertices; i++) {
		const auto t = f32(i) / f32(vertices - 1);
		const auto angle = lerp(startAngle, endAngle, t);
		const auto direction = Vec2::oriented(angle);
		const auto vertexInside = addVertex(pos + direction * insideRadius);
		const auto vertexOutside = addVertex(pos + direction * outsideRadius);

		addFilledQuad(oldVertexInside, oldVertexOutside, vertexOutside, vertexInside);

		oldVertexInside = vertexInside;
		oldVertexOutside = vertexOutside;
	}
}

void Gfx2d::circleArcTriangulated(Vec2 pos, f32 radius, f32 startAngle, f32 endAngle, f32 width, Vec3 color, i32 vertices) {
	circleArcTriangulated(pos, radius, startAngle, endAngle, width, Vec4(color, 1.0f), vertices);
}

void Gfx2d::circleArcTriangulated(Vec2 pos, f32 radius, f32 startAngle, f32 endAngle, f32 width, Vec4 color) {
	const auto vertices = i32((endAngle - startAngle) / TAU<f32> *calculateCircleVertexCount(radius));
	circleArcTriangulated(pos, radius, startAngle, endAngle, width, color, vertices);
}

void Gfx2d::circleArcTriangulated(Vec2 pos, f32 radius, f32 startAngle, f32 endAngle, f32 width, Vec3 color) {
	const auto vertices = i32((endAngle - startAngle) / TAU<f32> * calculateCircleVertexCount(radius));
	circleArcTriangulated(pos, radius, startAngle, endAngle, width, color, vertices);
}

void Gfx2d::disk(Vec2 pos, f32 radius, Vec3 color) {
	diskInstances.add(DiskInstance{
		.transform = camera.makeTransform(pos, 0.0f, Vec2(radius)),
		.color = Vec4(color, 1.0f),
	});
}

void Gfx2d::diskTriangulated(Vec2 pos, f32 radius, Vec4 color, i32 vertices) {
	auto addVertex = [&](Vec2 pos) -> u32 {
		const auto index = filledTrianglesVertices.size();
		filledTrianglesVertices.add(Vertex2Pc{ .position = pos, .color = color });
		return u32(index);
	};
	const auto center = addVertex(pos);
	const auto firstVertex = addVertex(pos + Vec2(radius, 0.0f));
	u32 oldVertex = firstVertex;
	for (i32 i = 1; i < vertices - 1; i++) {
		const auto t = f32(i) / f32(vertices - 1);
		const auto angle = lerp(0.0f, TAU<f32>, t);
		const auto newVertex = addVertex(pos + Vec2::fromPolar(angle, radius));
		addFilledTriangle(center, oldVertex, newVertex);
		oldVertex = newVertex;
	}
	addFilledTriangle(center, oldVertex, firstVertex);
}

void Gfx2d::diskTriangulated(Vec2 pos, f32 radius, Vec4 color) {
	diskTriangulated(pos, radius, color, calculateCircleVertexCount(radius));
}

void Gfx2d::line(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec3 color) {
	const auto vector = endpoint1 - endpoint0;
	const auto pixelWidth = getQuadPixelSizeY(width);
	const auto halfWidth = width / 2.0f;
	lineInstances.add(LineInstance{
		.transform = camera.makeTransform(endpoint0 + vector / 2.0f, vector.angle(), Vec2(vector.length() / 2.0f + halfWidth, halfWidth)),
		.color = Vec4(color, 1.0f),
		.smoothing = 3.0f / pixelWidth,
		.halfLineWidth = halfWidth,
		.lineLength = vector.length() + halfWidth * 2.0f,
	});
}

void Gfx2d::ray(Vec2 start, Vec2 direction, f32 width, Vec3 color) {
	line(start, start + direction, width, color);
}

void Gfx2d::arrow(Vec2 start, Vec2 end, f32 tipLength, f32 tipAngle, f32 width, Vec3 color) {
	line(start, end, width, color);
	const auto rotation = Rotation(tipAngle);
	const auto direction = (end - start).normalized();
	ray(end, rotation * -direction * tipLength, width, color);
	ray(end, rotation.inversed() * -direction * tipLength, width, color);
}

void Gfx2d::rect(Vec2 bottomLeft, Vec2 size, f32 width, Vec3 color) {
	Vec2 vertices[] = {
		bottomLeft, 
		Vec2(bottomLeft.x, bottomLeft.y + size.y), 
		bottomLeft + size, Vec2
		(bottomLeft.x + size.x, bottomLeft.y)
	};
	polygon(constView(vertices), width, color);
}

void Gfx2d::box(Vec2 center, Vec2 halfSize, f32 rotation, f32 width, Vec3 color) {
	const auto rot = Rotation(rotation);
	Vec2 vertices[] = {
		halfSize, 
		Vec2(halfSize.x, - halfSize.y),
		-halfSize,
		Vec2(-halfSize.x, halfSize.y)
	};
	for (auto& vertex : vertices) {
		vertex = center + rot * vertex;
	}
	polygon(constView(vertices), width, color);
}

void Gfx2d::polygon(View<const Vec2> vertices, f32 width, Vec3 color) {
	if (vertices.size() < 2) {
		return;
	}

	int previous = vertices.size() - 1;
	for (int i = 0; i < vertices.size(); previous = i, i++) {
		line(vertices[previous], vertices[i], width, color);
		previous = i;
	}
}

void Gfx2d::polyline(View<const Vec2> vertices, f32 width, Vec3 color) {
	if (vertices.size() < 2) {
		return;
	}
	for (i32 i = 1; i < vertices.size(); i++) {
		line(vertices[i - 1], vertices[i], width, color);
	}
}

void Gfx2d::polylineTriangulated(View<const Vec2> vertices, f32 width, Vec3 color, i32 endpointVertices) {
	if (vertices.size() < 2) {
		return;
	}
	for (i32 i = 1; i < vertices.size(); i++) {
		lineTriangulated(vertices[i - 1], vertices[i], width, color, endpointVertices);
	}
}

void Gfx2d::polylineTriangulated(View<const Vec2> vertices, f32 width, Vec3 color) {
	polylineTriangulated(vertices, width, color, calculateCircleVertexCount(width / 2.0f) / 2);
}

void Gfx2d::polygonTriangulated(View<const Vec2> vertices, f32 width, Vec3 color, i32 endpointVertices) {
	if (vertices.size() < 2) {
		return;
	}

	i64 previous = vertices.size() - 1;
	for (i64 i = 0; i < vertices.size(); previous = i, i++) {
		lineTriangulated(vertices[previous], vertices[i], width, color, endpointVertices);
		previous = i;
	}
}

void Gfx2d::polygonTriangulated(View<const Vec2> vertices, f32 width, Vec3 color) {
	polygonTriangulated(vertices, width, color, calculateCircleVertexCount(width / 2.0f) / 2);
}

void Gfx2d::filledTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Vec3 color) {
	const auto i0 = addFilledTriangleVertex(v0, Vec4(color, 1.0f));
	const auto i1 = addFilledTriangleVertex(v1, Vec4(color, 1.0f));
	const auto i2 = addFilledTriangleVertex(v2, Vec4(color, 1.0f));
	addFilledTriangle(i0, i1, i2);
}

void Gfx2d::filledTriangles(View<const Vec2> vertices, View<const i32> indices, Vec4 color) {
	const auto offset = filledTrianglesVertices.size();
	for (i64 i = 0; i < vertices.size(); i++) {
		filledTrianglesVertices.add(Vertex2Pc{ .position = vertices[i], .color = color });
	}

	for (i64 i = 0; i < indices.size(); i++) {
		filledTrianglesIndices.add(offset + indices[i]);
	}
}

void Gfx2d::filledTriangles(View<const Vec2> vertices, View<const i32> indices, Vec3 color) {
	filledTriangles(vertices, indices, Vec4(color, 1.0f));
}

void Gfx2d::filledRect(Vec2 center, Vec2 size, Vec3 color) {
	const auto halfSize = size / 2.0f;
	const auto i0 = addFilledTriangleVertex(center + halfSize, Vec4(color, 1.0f));
	const auto i1 = addFilledTriangleVertex(center + Vec2(halfSize.x, -halfSize.y), Vec4(color, 1.0f));
	const auto i2 = addFilledTriangleVertex(center - halfSize, Vec4(color, 1.0f));
	const auto i3 = addFilledTriangleVertex(center + Vec2(-halfSize.x, halfSize.y), Vec4(color, 1.0f));
	addFilledTriangle(i0, i1, i2);
	addFilledTriangle(i0, i2, i3);
}

void Gfx2d::lineTriangulated(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec3 color, i32 endpointVertices) {
	lineTriangulated(endpoint0, endpoint1, width, Vec4(color, 1.0f), endpointVertices);
}

void Gfx2d::lineTriangulated(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec4 color, i32 endpointVertices) {
	auto addVertex = [&](Vec2 pos) -> u32 {
		const auto index = filledTrianglesVertices.size();
		filledTrianglesVertices.add(Vertex2Pc{ .position = pos, .color = color });
		return u32(index);
	};
	const auto endpointToVertex = (endpoint1 - endpoint0).normalized().rotBy90deg() * width / 2.0f;
	const auto v00 = addVertex(endpoint0 - endpointToVertex);
	const auto v10 = addVertex(endpoint0 + endpointToVertex);
	const auto v01 = addVertex(endpoint1 - endpointToVertex);
	const auto v11 = addVertex(endpoint1 + endpointToVertex);

	/*
	v00--endpoint0--v10
	 |               |
	v01--endpoint1--v11
	*/
	addFilledTriangle(v01, v11, v10);
	addFilledTriangle(v01, v10, v00);

	const auto e0 = addVertex(endpoint0);
	const auto e1 = addVertex(endpoint1);

	const auto startAngle = endpointToVertex.angle();
	const auto endAngle = startAngle + PI<f32>;

	auto endpoint0PreviousVertex = v00;
	auto endpoint1PreviousVertex = v11;
	for (i32 i = 1; i < endpointVertices - 1; i++) {
		const auto t = f32(i) / f32(endpointVertices - 1);
		const auto angle = lerp(startAngle, endAngle, t);
		const auto offset = Vec2::fromPolar(angle, width / 2.0f);

		const auto newVertexAtEndpoint0 = addVertex(endpoint0 - offset);
		const auto newVertexAtEndpoint1 = addVertex(endpoint1 + offset);

		addFilledTriangle(e0, endpoint0PreviousVertex, newVertexAtEndpoint0);
		addFilledTriangle(e1, endpoint1PreviousVertex, newVertexAtEndpoint1);

		endpoint0PreviousVertex = newVertexAtEndpoint0;
		endpoint1PreviousVertex = newVertexAtEndpoint1;
	}
	addFilledTriangle(e0, endpoint0PreviousVertex, v10);
	addFilledTriangle(e1, endpoint1PreviousVertex, v01);
}

void Gfx2d::lineTriangulated(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec3 color) {
	lineTriangulated(endpoint0, endpoint1, width, color, calculateCircleVertexCount(width / 2.0f) / 2);
}

void Gfx2d::lineTriangulated(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec4 color) {
	lineTriangulated(endpoint0, endpoint1, width, color, calculateCircleVertexCount(width / 2.0f) / 2);
}

void Gfx2d::drawCircles() {
	circleShader.use();
	drawInstances(circleVao, instancesVbo, constView(circleInstances), quad2dPtDrawInstances);
	circleInstances.clear();
}

void Gfx2d::drawLines() {
	lineShader.use();
	drawInstances(lineVao, instancesVbo, constView(lineInstances), quad2dPtDrawInstances);
	lineInstances.clear();
}

void Gfx2d::drawDisks() {
	diskShader.use();
	drawInstances(diskVao, instancesVbo, constView(diskInstances), quad2dPtDrawInstances);
	diskInstances.clear();
}

void Gfx2d::drawFilledTriangles() {
	filledTrianglesVbo.allocateData(filledTrianglesVertices.data(), filledTrianglesVertices.byteSize());
	filledTrianglesIbo.allocateData(filledTrianglesIndices.data(), filledTrianglesIndices.byteSize());
	filledTrianglesVao.bind();
	filledTriangleShader.use();
	shaderSetUniforms(filledTriangleShader, FilledTriangleVertUniforms{
		.transform = camera.clipSpaceToWorldSpace().inversed()
	});

	glDrawElements(GL_TRIANGLES, filledTrianglesIndices.size(), GL_UNSIGNED_INT, nullptr);

	filledTrianglesIndices.clear();
	filledTrianglesVertices.clear();
}

#include <gfx2d/DbgGfx2d.hpp>

void Gfx2d::drawDebug() {
#ifndef FINAL_RELEASE
	for (const auto& disk : Dbg::disks) {
		Gfx2d::disk(disk.pos, disk.radius, disk.color);
	}
	drawDisks();

	for (const auto& line : Dbg::lines) {
		Gfx2d::line(line.e0, line.e1, line.width, line.color);
	}
	drawLines();
#endif
	Dbg::update();
}

f32 Gfx2d::getQuadPixelSizeY(f32 scale) {
	return scale * camera.zoom * Window::size().y;
}

i32 Gfx2d::calculateCircleVertexCount(f32 radius) const {
	// TODO: 
	//return i32(20 * radius * camera.zoom);
	return 70;
}
