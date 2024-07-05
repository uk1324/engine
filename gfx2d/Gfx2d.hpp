#pragma once

#include <engine/Graphics/Vbo.hpp>
#include <engine/Graphics/Ibo.hpp>
#include <gfx2d/Shaders/circleData.hpp>
#include <gfx2d/Shaders/lineData.hpp>
#include <gfx2d/Camera.hpp>

struct Gfx2d {
	static Gfx2d make();

	Vbo quad2dPtVbo;
	Ibo quad2dPtIbo;

	Vao circleVao;
	ShaderProgram& circleShader;
	std::vector<CircleInstance> circleInstances;

	Vao lineVao;
	ShaderProgram& lineShader;
	std::vector<LineInstance> lineInstances;
	
	void addCircle(Vec2 pos, f32 radius, f32 width, Vec3 color);
	void drawCircles();

	void addLine(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec3 color);
	void drawLines();

	f32 getQuadPixelSizeY(f32 scale);

	Camera camera;
	//Vao lineVao;
	//ShaderProgram& lineShader;
	//std::vector<LineInstance> lineInstances;

	Vbo instancesVbo;
};