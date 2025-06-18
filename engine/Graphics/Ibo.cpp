#include <engine/Graphics/Ibo.hpp>
#include <opengl/gl.h>

Ibo Ibo::generate() {
	u32 handle;
	glGenBuffers(1, &handle);
	return Ibo(handle);
}

Ibo::Ibo(const void* data, usize dataByteSize) {
	glGenBuffers(1, &handle_);
	allocateData(data, dataByteSize);
}

Ibo::~Ibo() {
	glDeleteBuffers(1, &handle_);
}

Ibo::Ibo(Ibo&& other) noexcept
	: handle_(other.handle_) {
	other.handle_ = NULL;
}

Ibo& Ibo::operator=(Ibo&& other) noexcept {
	glDeleteBuffers(1, &handle_);
	handle_ = other.handle_;
	other.handle_ = NULL;
	return *this;
}

#include "Vao.hpp"
Vao* testVao;

void Ibo::allocateData(const void* data, usize dataByteSize) {
	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound vao.
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of vao state. 
	// Breaks when using emscripten
	// https://stackoverflow.com/questions/44959697/why-doesnt-webgl-allow-binding-the-same-buffer-to-array-buffer-and-element-arra

	#ifdef __EMSCRIPTEN__
	if (testVao == nullptr) {
		testVao = new Vao(0);
		*testVao = Vao::generate();
	}
	testVao->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataByteSize, data, GL_STATIC_DRAW);
	#else
	glBindBuffer(GL_ARRAY_BUFFER, handle_);
	glBufferData(GL_ARRAY_BUFFER, dataByteSize, data, GL_STATIC_DRAW);
	#endif

}

void Ibo::allocateData(View<const i32> indices) {
	allocateData(indices.data(), indices.size() * sizeof(i32));
}

void Ibo::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
}

void Ibo::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Ibo::Ibo(u32 handle)
	: handle_(handle) {}
