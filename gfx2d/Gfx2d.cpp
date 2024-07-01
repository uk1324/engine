#include "Gfx2d.hpp"
#include "Quad2dPt.hpp"
#include "StructUtils.hpp"



Gfx2d Gfx2d::make() {
	Vbo quad2dPtVbo(quad2dPtVertices, sizeof(quad2dPtVertices));
	Ibo quad2dPtIbo(quad2dPtIndices, sizeof(quad2dPtIndices));

	return Gfx2d{
		MOVE(quad2dPtVbo),
		MOVE(quad2dPtIbo),
		.instancesVbo = Vbo(1024ull * 10)
	};
}
