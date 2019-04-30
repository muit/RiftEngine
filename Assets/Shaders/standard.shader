## VERTEX ##

#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_normal;
layout (location = 2) in vec2 v_uv;

uniform mat4 mvp;

out vec2 uv;

void main()
{
	gl_Position = mvp * vec4(v_position, 1.0);
	uv = v_uv;
}



## FRAGMENT ##

#version 330 core

in vec2 uv;

void main()
{
	gl_FragColor = vec4(0.9, 0.1, 0.1, 1.0);
}
