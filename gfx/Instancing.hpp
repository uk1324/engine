#pragma once
#include <engine/Graphics/Vao.hpp>
#include <engine/Graphics/Vbo.hpp>
#include <engine/Graphics/Ibo.hpp>
#include <View.hpp>
#include <vector>
#include <opengl/gl.h>

//struct CreateInstancingVaoResult {
//	Vao vao;
//	Ibo ibo;
//};
//
//template<typename Shader>
//static Vao createInstancingVao(Vbo& verticesVbo, void* indiciesData, i32 indicesDataSize, Vbo& instancesVbo) {
//	auto vao = Vao::generate();
//	Shader::addAttributesToVao(vao, verticesVbo, instancesVbo);
//	vao.bind();
//	// 
//	if (indiciesData == nullptr) {
//		Ibo ibo = Ibo::generate();
//		ibo.bind();
//		Vao::unbind();
//		Ibo::unbind();
//		return { std::move(vao), std::move(ibo) };
//	} else {
//		Ibo ibo(indiciesData, indicesDataSize);
//		ibo.bind();
//		Vao::unbind();
//		Ibo::unbind();
//		return { std::move(vao), std::move(ibo) };
//	}
//
//	/*verticesIbo.bind();
//	Vao::unbind();
//	Ibo::unbind();*/
//}


template<typename Shader>
static Vao createInstancingVao(Vbo& verticesVbo, Ibo& verticesIbo, Vbo& instancesVbo) {
	auto vao = Vao::generate();
	Shader::addAttributesToVao(vao, verticesVbo, instancesVbo);
	vao.bind();
	verticesIbo.bind();
	Vao::unbind();
	Ibo::unbind();
	return vao;
}

template<typename Instance, typename DrawFunction>
static void drawInstances(Vao& vao, Vbo& instancesVbo, const View<const Instance>& instances, DrawFunction drawFunction) {
	vao.bind();
	instancesVbo.bind();

	// @Performance?
	GLint64 instanceBufferSize;
	glGetBufferParameteri64v(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &instanceBufferSize);

	const auto maxInstancesPerDrawCall = i32(instanceBufferSize / sizeof(Instance));
	i32 drawn = 0;
	while (drawn < instances.size()) {
		const auto leftToDraw = i32(instances.size()) - drawn;
		const auto toDrawInThisDrawCall = (leftToDraw > maxInstancesPerDrawCall) ? maxInstancesPerDrawCall : leftToDraw;
		boundVboSetData(0, instances.data() + drawn, toDrawInThisDrawCall * sizeof(Instance));
		drawFunction(toDrawInThisDrawCall);
		drawn += toDrawInThisDrawCall;
	}
}