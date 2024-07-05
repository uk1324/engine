#version 430 core

in vec2 position; 

in vec4 color; 
in float smoothing; 
in float width; 
out vec4 fragColor;

/*generated end*/

#include "../../gfx/sdf.glsl"

void main() {
	vec2 p = position;
	
	float d = distance(vec2(0), position);
	d -= 1.0 - smoothing;

	float d1 = distance(vec2(0), position);
	d1 -= 1.0 - width - smoothing;
	d = subtractSdf(d, d1);
	d = smoothstep(smoothing, 0.0, d);

//	fragColor = vec4(vec3(d), 1.0);
//	return;
	fragColor = vec4(color.rgb, d * color.a);
}
