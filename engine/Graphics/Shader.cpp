#include <engine/Graphics/Shader.hpp>
#include <FileIo.hpp>
#include <Overloaded.hpp>
#include <engine/Log.hpp>
#include <opengl/gl.h>
#include <filesystem>

std::expected<Shader, Shader::Error> Shader::compile(std::string_view path, ShaderType type) {
	auto source = preprocessIncludes(path);
	if (!source.has_value()) {
		return std::unexpected(Error{ std::move(source.error()) });
	}
	return fromSource(source->c_str(), type);
}

std::expected<Shader, Shader::Error> Shader::fromSource(std::string_view source, ShaderType type) {
	#ifdef __EMSCRIPTEN__
	const auto firstLineEnd = source.find_first_of('\n');
	const auto withoutFirstLine = source.substr(firstLineEnd);
	const auto s = std::string("#version 300 es\nprecision mediump float;\n") + std::string(withoutFirstLine);
	source = std::string_view(s);
	//put("%", source);
	#endif

	const auto handle = glCreateShader(static_cast<GLenum>(type));
	const char* src = source.data();
	const auto length = static_cast<GLint>(source.length());

	glShaderSource(handle, 1, &src, &length);
	glCompileShader(handle);

	GLint status;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		char infoLog[512];
		glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
		return std::unexpected(CompileError{ infoLog });
	}
	return Shader(handle);
}

Shader::Shader(Shader&& other) noexcept
	: handle_(other.handle_) {
	other.handle_ = NULL;
}

Shader& Shader::operator=(Shader&& other) noexcept {
	glDeleteShader(handle_);
	handle_ = other.handle_;
	other.handle_ = NULL;
	return *this;
}

Shader::~Shader() {
	glDeleteShader(handle_);
}

GLuint Shader::handle() const {
	return handle_;
}

Shader::Shader(u32 handle)
	: handle_(handle){}

std::ostream& operator<<(std::ostream& os, const Shader::Error& e) {
	std::visit(overloaded{
		[&](const PreprocessIncludesError& e) {
			os << e;
		},
		[&](const Shader::CompileError& e) {
			os << e.message;
		}
	}, e);
	return os;
}
