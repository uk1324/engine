#pragma once

#include <engine/Graphics/Vbo.hpp>
#include <engine/Graphics/Ibo.hpp>

struct Gfx2d {
	static Gfx2d make();

	Vbo quad2dPtVbo;
	Ibo quad2dPtIbo;

	Vbo instancesVbo;
};