#include <engine/Graphics/ShaderProgram.hpp>
#include <engine/Log.hpp>
#include <opengl/gl.h>
#include <sstream>

std::expected<ShaderProgram, ShaderProgram::Error> ShaderProgram::tryCompile(std::string_view vertexPath, std::string_view fragmentPath) {
	auto vertex = Shader::compile(vertexPath, ShaderType::Vertex);
	auto fragment = Shader::compile(fragmentPath, ShaderType::Fragment);
	return fromShaders(vertex, fragment);
}

std::expected<ShaderProgram, ShaderProgram::Error> ShaderProgram::fromSource(std::string_view vertSource, std::string_view fragSource) {
	auto vertex = Shader::fromSource(vertSource, ShaderType::Vertex);
	auto fragment = Shader::fromSource(fragSource, ShaderType::Fragment);
	return fromShaders(vertex, fragment);
}

ShaderProgram ShaderProgram::compile(std::string_view vertexPath, std::string_view fragmentPath) {
	auto shader = tryCompile(vertexPath, fragmentPath);
	if (shader.has_value()) {
		return std::move(*shader);
	}
	std::stringstream s;
	s << shader.error();
	Log::fatal("failed to compile vert = '%' frag = '%': %", vertexPath.data(), fragmentPath.data(), s.str().c_str());
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(handle_);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	: handle_(other.handle_) {
	other.handle_ = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
	glDeleteProgram(handle_);
	handle_ = other.handle_;
	other.handle_ = 0;
	m_cachedUniformLocations = std::move(other.m_cachedUniformLocations);
	return *this;
}

void ShaderProgram::addShader(const Shader& shader) {
	glAttachShader(handle_, shader.handle());
}

std::optional<std::string> ShaderProgram::link() {
	glLinkProgram(handle_);
	GLint status;
	glGetProgramiv(handle_, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		char infoLog[512];
		glGetProgramInfoLog(handle_, sizeof(infoLog), nullptr, infoLog);
		return infoLog;
	}
	return std::nullopt;
}

#include <iostream>

void ShaderProgram::use() {
	//printf("%d", handle_);
	//std::cout << handle_ << '\n';
	//put("before use");
	glUseProgram(handle_);
	//put("after use");
}

// Using glProgramUniform instead of glUniform might be sometimes faster because it doesn't need a glUsePgoram call before it.
void ShaderProgram::set(std::string_view name, const Vec2& vec) {
#ifdef __EMSCRIPTEN__
	use();
	glUniform2fv(getUniformLocation(name.data()), 1, vec.data());
#else
	glProgramUniform2fv(handle_, getUniformLocation(name.data()), 1, vec.data());
#endif

}

void ShaderProgram::set(std::string_view name, const Vec3& vec) {
#ifdef __EMSCRIPTEN__
	use();
	glUniform3fv(getUniformLocation(name.data()), 1, vec.data());
#else
	glProgramUniform3fv(handle_, getUniformLocation(name.data()), 1, vec.data());
#endif
}

void ShaderProgram::set(std::string_view name, const Vec4& vec) {
#ifdef __EMSCRIPTEN__
	use();
	glUniform4fv(getUniformLocation(name.data()), 1, vec.data());
#else
	glProgramUniform4fv(handle_, getUniformLocation(name.data()), 1, vec.data());
#endif
}

//void ShaderProgram::setVec3I(std::string_view name, const Vec3I& vec)
//{
//	glProgramUniform3iv(m_handle, getUniformLocation(name.data()), 1, vec.data());
//}
//
//void ShaderProgram::setMat4(std::string_view name, const Mat4& mat)
//{
//	glProgramUniformMatrix4fv(m_handle, getUniformLocation(name.data()), 1, GL_FALSE, mat.data());
//}

void ShaderProgram::set(std::string_view name, int32_t value) {
#ifdef __EMSCRIPTEN__
	use();
	glUniform1i(getUniformLocation(name.data()), value);
#else
	glProgramUniform1i(handle_, getUniformLocation(name.data()), value);
#endif
}


#ifndef __EMSCRIPTEN__
void ShaderProgram::set(std::string_view name, uint32_t value) {
#ifdef __EMSCRIPTEN__
	use();
	glUniform1ui(handle_, getUniformLocation(name.data()), value);
#else
	glProgramUniform1ui(handle_, getUniformLocation(name.data()), value);
#endif
}
#endif

void ShaderProgram::setTexture(std::string_view name, int index, Texture& texture, u32 target) {
	setTexture(name, index, texture.handle(), target);
}

void ShaderProgram::setTexture(std::string_view name, int index, u32 textureHandle, u32 target) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(target, textureHandle);
	set(name, index);
}

// TODO make a utility function that takes the actual texture and an index and call glActiveTexture.
void ShaderProgram::setActiveTexture(std::string_view name, int value) {
	set(name, value);
}

void ShaderProgram::set(std::string_view name, float value) {
#ifdef __EMSCRIPTEN__
	use();
	glUniform1f(getUniformLocation(name.data()), value);
#else
	glProgramUniform1f(handle_, getUniformLocation(name.data()), value);
#endif
}

void ShaderProgram::set(std::string_view name, bool value) {
	// There is no special function for bools.
	set(name, static_cast<i32>(value));
}

void ShaderProgram::set(std::string_view name, const Mat3x2& value) {
#ifdef __EMSCRIPTEN__
	use();
	glUniformMatrix3x2fv(getUniformLocation(name), 1, false, reinterpret_cast<const float*>(value.m));
#else
	glProgramUniformMatrix3x2fv(handle_, getUniformLocation(name), 1, false, reinterpret_cast<const float*>(value.m));
#endif
}

void ShaderProgram::set(std::string_view name, const Mat3& value) {
#ifdef __EMSCRIPTEN__
	use();
	glUniformMatrix3fv(getUniformLocation(name.data()), 1, GL_FALSE, value.data());
#else
	glProgramUniformMatrix3fv(handle_, getUniformLocation(name.data()), 1, GL_FALSE, value.data());
#endif
}

void ShaderProgram::set(std::string_view name, const Mat4& value) {
#ifdef __EMSCRIPTEN__
	use();
	glUniformMatrix4fv(getUniformLocation(name.data()), 1, GL_FALSE, value.data());
#else
	glProgramUniformMatrix4fv(handle_, getUniformLocation(name.data()), 1, GL_FALSE, value.data());
#endif
}

void ShaderProgram::set(std::string_view name, std::span<const Vec2> vecs) {
#ifdef __EMSCRIPTEN__
	use();
	glUniform2fv(getUniformLocation(name.data()), GLsizei(vecs.size()), reinterpret_cast<const float*>(vecs.data()));
#else
	glProgramUniform2fv(handle_, getUniformLocation(name.data()), GLsizei(vecs.size()), reinterpret_cast<const float*>(vecs.data()));
#endif
}

GLuint ShaderProgram::handle() const {
	return handle_;
}

std::expected<ShaderProgram, ShaderProgram::Error> ShaderProgram::fromShaders(
	std::expected<Shader, Shader::Error>& fragment,
	std::expected<Shader, Shader::Error>& vertex) {

	if (!vertex.has_value() || !fragment.has_value()) {
		return std::unexpected(ShaderProgram::Error{
			.vertexError = !vertex.has_value() ? std::optional(std::move(vertex.error())) : std::nullopt,
			.fragmentError = !fragment.has_value() ? std::optional(std::move(fragment.error())) : std::nullopt,
			});
	}

	ShaderProgram program(glCreateProgram());
	//std::cout << "created shader: " << program.handle() << '\n';

	program.addShader(*vertex);
	program.addShader(*fragment);
	auto linkerError = program.link();
	if (linkerError.has_value()) {
		return std::unexpected(ShaderProgram::Error{ .linkerErrorMessage = std::move(*linkerError) });
	}
	return program;
}

ShaderProgram::ShaderProgram(u32 handle)
	: handle_(handle) {
}

ShaderProgram ShaderProgram::null() {
	return ShaderProgram(0);
}

int ShaderProgram::getUniformLocation(std::string_view name) {
	// Can't assume that the string_view data won't get destroyed.
	std::string uniformName(name);
	//put("looking for uniform %", name);
	auto location = m_cachedUniformLocations.find(uniformName);
	if (location == m_cachedUniformLocations.end())
	{
		int location = glGetUniformLocation(handle_, uniformName.c_str());
		if (location == -1)
			Log::warning("trying to set variable '%', which doesn't exist", name);

		m_cachedUniformLocations[std::move(uniformName)] = location;
		return location;
	}
	return location->second;
}

std::ostream& operator<<(std::ostream& os, const ShaderProgram::Error& e) {
	if (e.vertexError.has_value()) {
		os << "vert error: " << *e.vertexError << '\n';
	}
	if (e.fragmentError.has_value()) {
		os << "frag error: " << *e.fragmentError << '\n';
	}
	if (e.linkerErrorMessage.has_value()) {
		os << "linker error: " << *e.linkerErrorMessage << '\n';
	}
	return os;
}
