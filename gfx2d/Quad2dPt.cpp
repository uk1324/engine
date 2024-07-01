#include "Quad2dPt.hpp"
#include "glad/glad.h"

void quad2dPtDrawInstances(usize count) {
	glDrawElementsInstanced(GL_TRIANGLES, GLsizei(6), GL_UNSIGNED_INT, nullptr, GLsizei(count));
}
