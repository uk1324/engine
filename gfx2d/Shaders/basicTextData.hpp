#pragma once
#include <engine/Math/Vec2.hpp> 
#include <engine/Math/Vec4.hpp> 
#include <engine/Graphics/ShaderProgram.hpp> 
#include <vector> 
#include <engine/Graphics/Vao.hpp> 

struct BasicTextInstance {
    Mat3x2 transform; 
    Vec2 offsetInAtlas; 
    Vec2 sizeInAtlas; 
    float smoothing; 
    Vec4 color; 
    float offset; 
};


struct BasicTextFragUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const BasicTextFragUniforms& value);


struct BasicTextVertUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const BasicTextVertUniforms& value);



// For template use.
struct BasicTextShader {
    static void addAttributesToVao(Vao& vao, Vbo& verticesVbo, Vbo& instancesVbo);
    // Could put the path constants here
};

static constexpr const char* BASIC_TEXT_SHADER_VERT_PATH = "engine/gfx2d/Shaders/basicText.vert";
static constexpr const char* BASIC_TEXT_SHADER_FRAG_PATH = "engine/gfx2d/Shaders/basicText.frag"; 