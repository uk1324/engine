#include "GramSchmidt.hpp"

void gramSchmidtOrthonormalize(View<Vec4> basis) {
	basis[0] = basis[0].normalized();
	for (i32 i = 1; i < basis.size(); i++) {
		auto& v = basis[i];
		for (i32 j = 0; j < i; j++) {
			v -= dot(v, basis[j]) * basis[j];
		}
		v = v.normalized();
	}
}