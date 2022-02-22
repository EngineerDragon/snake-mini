#version 330 core
layout (location = 0) in float unused;
uniform ivec2 tile;
void main()
{
	const float offset = 1.0 - 1.0/30.0;
	gl_Position = vec4(tile.x / 15.0 - offset,- tile.y / 15.0 + offset, 0.0, 1.0);
}
