#pragma once
#include <engine/Math/Vec4.hpp> 
#include <engine/Math/Vec2.hpp> 
#include <engine/Graphics/ShaderProgram.hpp> 
#include <vector> 
#include <engine/Graphics/Vao.hpp> 

struct LineInstance {
    Mat3x2 transform; 
    Vec4 color; 
    float smoothing; 
    float halfLineWidth; 
    float lineLength; 
};


struct LineFragUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const LineFragUniforms& value);


struct LineVertUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const LineVertUniforms& value);



// For template use.
struct LineShader {
    static void addAttributesToVao(Vao& vao, Vbo& verticesVbo, Vbo& instancesVbo);
    // Could put the path constants here
};

static constexpr const char* LINE_SHADER_VERT_PATH = "engine/gfx2d/Shaders/line.vert";
static constexpr const char* LINE_SHADER_FRAG_PATH = "engine/gfx2d/Shaders/line.frag"; 