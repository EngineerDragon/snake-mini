#version 330 core
out vec4 col;
uniform float time;
uniform bool isfood;
vec3 hsv2rgb(vec3 c)
{ 
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0); 
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
void main() {
	if (isfood) { 
		col = vec4(1.0f); 
	} else 
	{
		col = vec4(hsv2rgb(vec3(sin(time),1.0,1.0)),1.0);
	}	
}
