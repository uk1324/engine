#pragma once

#include <engine/Graphics/Vbo.hpp>
#include <engine/Graphics/Ibo.hpp>
#include <View.hpp>
#include <gfx2d/Shaders/circleData.hpp>
#include <gfx2d/Shaders/lineData.hpp>
#include <gfx2d/Shaders/diskData.hpp>
#include <gfx2d/Shaders/filledTriangleData.hpp>
#include <gfx2d/Camera.hpp>
#include <List.hpp>

struct Gfx2d {
	static Gfx2d make();

	Vbo quad2dPtVbo;
	Ibo quad2dPtIbo;

	Vao circleVao;
	ShaderProgram& circleShader;
	List<CircleInstance> circleInstances;

	Vao diskVao;
	ShaderProgram& diskShader;
	List<DiskInstance> diskInstances;

	Vao lineVao;
	ShaderProgram& lineShader;
	List<LineInstance> lineInstances;
	
	List<u32> filledTrianglesIndices;
	i32 addFilledTriangleVertex(Vec2 pos, Vec4 color);
	void addFilledTriangle(u32 i0, u32 i1, u32 i2);
	// the input should be the vertices ordered counterclockwise.
	void addFilledQuad(u32 i0, u32 i1, u32 i2, u32 i3);
	// @Performance: Instead of using `Vec4`s for colors, could use 32 bit rgba.
	List<Vertex2Pc> filledTrianglesVertices;

	Vbo filledTrianglesVbo;
	Ibo filledTrianglesIbo;
	Vao filledTrianglesVao;
	ShaderProgram& filledTriangleShader;

	void circle(Vec2 pos, f32 radius, f32 width, Vec3 color);
	void circleTriangulated(Vec2 pos, f32 radius, f32 width, Vec3 color, i32 vertices);
	void circleTriangulated(Vec2 pos, f32 radius, f32 width, Vec3 color);
	void circleArcTriangulated(Vec2 pos, f32 radius, f32 startAngle, f32 endAngle, f32 width, Vec3 color, i32 vertices);
	void circleArcTriangulated(Vec2 pos, f32 radius, f32 startAngle, f32 endAngle, f32 width, Vec3 color);
	void disk(Vec2 pos, f32 radius, Vec3 color);
	void diskTriangulated(Vec2 pos, f32 radius, Vec4 color, i32 vertices);
	void diskTriangulated(Vec2 pos, f32 radius, Vec4 color);
	void line(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec3 color);
	void ray(Vec2 start, Vec2 direction, f32 width, Vec3 color);
	void arrow(Vec2 start, Vec2 end, f32 tipLength, f32 tipAngle, f32 width, Vec3 color);
	void rect(Vec2 bottomLeft, Vec2 size, f32 width, Vec3 color);
	void box(Vec2 center, Vec2 halfSize, f32 rotation, f32 width, Vec3 color);
	void polygon(View<const Vec2> vertices, f32 width, Vec3 color);
	void polyline(View<const Vec2> vertices, f32 width, Vec3 color);
	void polylineTriangulated(View<const Vec2> vertices, f32 width, Vec3 color, i32 endpointVertices);
	void polylineTriangulated(View<const Vec2> vertices, f32 width, Vec3 color);
	void polygonTriangulated(View<const Vec2> vertices, f32 width, Vec3 color, i32 endpointVertices);
	void polygonTriangulated(View<const Vec2> vertices, f32 width, Vec3 color);
	void filledTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Vec4 color);
	void filledTriangle(Vec2 v0, Vec2 v1, Vec2 v2, Vec3 color);
	void filledTriangles(View<const Vec2> vertices, View<const i32> indices, Vec4 color);
	void filledTriangles(View<const Vec2> vertices, View<const i32> indices, Vec3 color);
	void lineTriangulated(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec3 color, i32 endpointVertices);
	void lineTriangulated(Vec2 endpoint0, Vec2 endpoint1, f32 width, Vec3 color);

	void drawCircles();
	void drawLines();
	void drawDisks();
	void drawFilledTriangles();

	f32 getQuadPixelSizeY(f32 scale);
	i32 calculateCircleVertexCount(f32 radius) const;

	Camera camera;

	Vbo instancesVbo;

};