#version 430 core

in vec2 texturePosition; 
in vec2 atlasMin; 
in vec2 atlasMax; 

in float smoothing; 
in vec4 color; 
in float offset; 
out vec4 fragColor;

/*generated end*/

uniform sampler2D fontAtlas;

float getD(vec2 p) {
	p = clamp(p, atlasMin, atlasMax);
	float d = texture(fontAtlas, p).r;
	float smoothingWidth = min(smoothing, 0.49);
	d -= 0.5 - smoothingWidth;
	d = smoothstep(0.0, smoothingWidth, d);	
	return d;
}

void main() {
	float smoothingWidth = min(smoothing, 0.49);
	float d = getD(texturePosition).r;
	vec3 col = vec3(d);
	fragColor = vec4(col, d);
}