#pragma once
#include <engine/Math/Vec4.hpp> 
#include <engine/Math/Vec2.hpp> 
#include <engine/Graphics/ShaderProgram.hpp> 
#include <vector> 
#include <engine/Graphics/Vao.hpp> 

struct DiskInstance {
    Mat3x2 transform; 
    Vec4 color; 
    float smoothing; 
};


struct DiskFragUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const DiskFragUniforms& value);


struct DiskVertUniforms {
};

void shaderSetUniforms(ShaderProgram& shader, const DiskVertUniforms& value);



// For template use.
struct DiskShader {
    static void addAttributesToVao(Vao& vao, Vbo& verticesVbo, Vbo& instancesVbo);
    // Could put the path constants here
};

static constexpr const char* DISK_SHADER_VERT_PATH = "engine/gfx2d/Shaders/disk.vert";
static constexpr const char* DISK_SHADER_FRAG_PATH = "engine/gfx2d/Shaders/disk.frag"; 