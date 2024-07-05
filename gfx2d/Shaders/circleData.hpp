#pragma once
#include <engine/Math/Vec4.hpp> 
#include <engine/Math/Vec2.hpp> 
#include <engine/Graphics/ShaderProgram.hpp> 
#include <vector> 
#include <engine/Graphics/Vao.hpp> 

struct CircleInstance {
    Mat3x2 transform; 
    Vec4 color; 
    float smoothing; 
    float width; 
};


struct CircleFragUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const CircleFragUniforms& value);


struct CircleVertUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const CircleVertUniforms& value);



// For template use.
struct CircleShader {
    static void addAttributesToVao(Vao& vao, Vbo& verticesVbo, Vbo& instancesVbo);
    // Could put the path constants here
};

static constexpr const char* CIRCLE_SHADER_VERT_PATH = "engine/gfx2d/Shaders/circle.vert";
static constexpr const char* CIRCLE_SHADER_FRAG_PATH = "engine/gfx2d/Shaders/circle.frag"; 