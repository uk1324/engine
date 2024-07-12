#pragma once
#include <engine/Math/Vec2.hpp> 
#include <engine/Graphics/ShaderProgram.hpp> 
#include <vector> 
#include <engine/Graphics/Vao.hpp> 

struct Vertex2Pc {
    Vec2 position; 
    Vec4 color; 
};

 
struct FilledTriangleInstance {
};


struct FilledTriangleFragUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const FilledTriangleFragUniforms& value);


struct FilledTriangleVertUniforms {
    Mat3x2 transform; 
};

void shaderSetUniforms(ShaderProgram& shader, const FilledTriangleVertUniforms& value);



// For template use.
struct FilledTriangleShader {
    static void addAttributesToVao(Vao& vao, Vbo& verticesVbo, Vbo& instancesVbo);
    // Could put the path constants here
};

static constexpr const char* FILLED_TRIANGLE_SHADER_VERT_PATH = "engine/gfx2d/Shaders/filledTriangle.vert";
static constexpr const char* FILLED_TRIANGLE_SHADER_FRAG_PATH = "engine/gfx2d/Shaders/filledTriangle.frag"; 