#version 430 core

in vec2 position; 

in vec4 color; 
in float smoothing; 
in float halfLineWidth; 
in float lineLength; 
out vec4 fragColor;

/*generated end*/

void main() {
	vec2 point = vec2(position.x * lineLength, position.y * halfLineWidth);

	float endpoint0X = halfLineWidth;
	float endpoint1X = lineLength - halfLineWidth;
	vec2 pointProjectedOntoLine = vec2(clamp(point.x, endpoint0X, endpoint1X), 0.0);
	float d = distance(point, pointProjectedOntoLine);

	float smoothing = fwidth(d) * 2.0;
	d -= halfLineWidth - smoothing;
	d = smoothstep(smoothing, 0.0, d);

	fragColor = vec4(color.rgb, d);
}
