#include "FontRenderer.hpp"
#include <glad/glad.h>
#include <gfx/Instancing.hpp>
#include <gfx/ShaderManager.hpp>
#include <engine/Window.hpp>
#include <gfx2d/Quad2dPt.hpp>
#include <engine/Log.hpp>

FontRenderer::FontRenderer(
	Vbo& fullscreenQuad2dPtVerticesVbo,
	Ibo& fullscreenQuad2dPtVerticesIbo,
	Vbo& instancesVbo)
	: basicTextShader(MAKE_GENERATED_SHADER(BASIC_TEXT))
	, basicTextVao(createInstancingVao<BasicTextShader>(fullscreenQuad2dPtVerticesVbo, fullscreenQuad2dPtVerticesIbo, instancesVbo)) {
}

void FontRenderer::render(Font& font, Vbo& instancesVbo) {
	basicTextShader.use();
	basicTextShader.setTexture("fontAtlas", 0, font.fontAtlas);
	drawInstances(basicTextVao, instancesVbo, constView(basicTextInstances), quad2dPtDrawInstances);
	basicTextInstances.clear();
}

void FontRenderer::addTextToDraw(
	const Font& font,
	Vec2 pos,
	const Mat3x2& transform,
	float maxHeight,
	std::string_view utf8Text,
	Vec4 color) {

	TextRenderInfoIterator iterator(font, pos, transform, maxHeight, utf8Text);
	for (auto info = iterator.next(); info.has_value(); info = iterator.next()) {
		basicTextInstances.push_back(BasicTextInstance{
			.transform = info->transform,
			.offsetInAtlas = info->offsetInAtlas,
			.sizeInAtlas = info->sizeInAtlas,
			.color = color,
		});
	}
}
