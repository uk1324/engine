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
	
	float dist = length(position);
	float smoothing = fwidth(dist) * 2.0;

	float d0 = dist;
	d0 -= 1.0;

	float d1 = dist;
	d1 -= 1.0 - width;

	float d;
	d = subtractSdf(d0, d1);
	d += smoothing;
	d = smoothstep(smoothing, 0.0, d);

	fragColor = vec4(color.rgb, d * color.a);
}
