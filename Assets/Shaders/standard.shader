## VERTEX ##

#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_normal;
layout (location = 2) in vec2 v_uv;

uniform mat4 mvp;
uniform mat4 model;

out vec3 normal
out vec3 world_position
out vec2 uv;

void main()
{
	const vec4 v_position4 = vec4(v_position, 1.0);

	world_position =  vec3(model * v_position4);
	gl_Position = mvp * v_position4;

    normal = mat3(model) * v_normal;
	uv = v_uv;
}


## FRAGMENT ##

#version 330 core

in vec3 normal
in vec3 world_position
in vec2 uv;

// material parameters
uniform vec3 base_color;
uniform float metallic;
uniform float roughness;
uniform float ao;

const float PI = 3.14159265359;

void main()
{
	gl_FragColor = vec4(0.9, 0.1, 0.1, 1.0);
}
