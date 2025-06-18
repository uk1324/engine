#include "Quad2dPt.hpp"
#include <opengl/gl.h>

void quad2dPtDrawInstances(usize count) {
	glDrawElementsInstanced(GL_TRIANGLES, GLsizei(6), GL_UNSIGNED_INT, nullptr, GLsizei(count));
}

void quad2dPtDraw() {
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
