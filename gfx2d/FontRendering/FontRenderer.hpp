#pragma once

#include "Font.hpp"
#include <string_view>
#include <gfx2d/Shaders/basicTextData.hpp>
#include <engine/Math/Vec4.hpp>
#include <engine/Graphics/Vao.hpp>
#include <gfx2d/Camera.hpp>
#include <engine/Graphics/Ibo.hpp>

struct FontRenderer {
	FontRenderer(
		Vbo& fullscreenQuad2dPtVerticesVbo,
		Ibo& fullscreenQuad2dPtVerticesIbo,
		Vbo& instancesVbo);

	void render(Font& font, Vbo& instancesVbo);

	ShaderProgram& basicTextShader;
	Vao basicTextVao;

	std::vector<BasicTextInstance> basicTextInstances;

	void addTextToDraw(
		const Font& font,
		Vec2 pos,
		const Mat3x2& transform,
		float maxHeight,
		std::string_view utf8Text,
		Vec4 color = Vec4(1.0f));
	struct TextInfo {
		Vec2 size;
		float bottomY;
	};
};