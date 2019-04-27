## VERTEX ##

#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_uv;

out vec2 uv;

uniform mat4 u_transform;

void main()
{
	gl_Position = u_transform * vec4(vertex_position, 1.0);
	uv = vertex_uv;
}


## FRAGMENT ##

#version 330 core

in vec2 uv;
layout(location = 0) out vec3 color;

uniform vec3 u_color;


void main()
{
	color = color;
}
