#include "Gfx2d.hpp"
#include "Quad2dPt.hpp"
#include "StructUtils.hpp"
#include <gfx/ShaderManager.hpp>
#include <gfx/Instancing.hpp>
#include <engine/Window.hpp>

Gfx2d Gfx2d::make() {
	auto instancesVbo = Vbo(1024ull * 10);

	Vbo quad2dPtVbo(quad2dPtVertices, sizeof(quad2dPtVertices));
	Ibo quad2dPtIbo(quad2dPtIndices, sizeof(quad2dPtIndices));

	auto circleVao = createInstancingVao<CircleShader>(quad2dPtVbo, quad2dPtIbo, instancesVbo);

	auto lineVao = createInstancingVao<LineShader>(quad2dPtVbo, quad2dPtIbo, instancesVbo);

	return Gfx2d{
		MOVE(quad2dPtVbo),
		MOVE(quad2dPtIbo),
		MOVE(circleVao),
		.circleShader = MAKE_GENERATED_SHADER(CIRCLE),
		MOVE(lineVao),
		.lineShader = MAKE_GENERATED_SHADER(LINE),
		MOVE(instancesVbo)
	};
}

void Gfx2d::addCircle(Vec2 pos, f32 radius, f32 width, Vec3 color) {
	const auto pixelSize = getQuadPixelSizeY(radius);
	circleInstances.push_back(CircleInstance{
		.transform = camera.makeTransform(pos, 0.0f, Vec2(radius)),
		.color = Vec4(color, 1.0f),
		.smoothing = 3.0f / pixelSize,
		.width = width / radius
	});
}

void Gfx2d::drawCircles() {
	circleShader.use();
	drawInstances(circleVao, instancesVbo, circleInstances, quad2dPtDrawInstances);
	circleInstances.clear();
}

void Gfx2d::addLine(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec3 color) {
	const auto vector = endpoint1 - endpoint0;
	const auto pixelWidth = getQuadPixelSizeY(width);
	lineInstances.push_back(LineInstance{
		.transform = camera.makeTransform(endpoint0 + vector / 2.0f, vector.angle(), Vec2(vector.length() / 2.0f + width, width)),
		.color = Vec4(color, 1.0f),
		.smoothing = 3.0f / pixelWidth,
		.lineWidth = width,
		.lineLength = vector.length() + width * 2.0f,
	});
}

void Gfx2d::drawLines() {
	lineShader.use();
	drawInstances(lineVao, instancesVbo, lineInstances, quad2dPtDrawInstances);
	lineInstances.clear();
}

f32 Gfx2d::getQuadPixelSizeY(f32 scale) {
	return scale * camera.zoom * Window::size().y;
}
