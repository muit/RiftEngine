## VERTEX ##

#version 330

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 vertex_uv;

out vec2 UV;

void main()
{
	gl_Position = vec4(vertex_position, 1.0);
	UV  = vertex_uv;
}


## FRAGMENT ##

#version 330

uniform sampler2D square2d;

in  vec2 UV;
out vec3 color;

void main()
{
	color = texture(square2d, vec2(UV.st.x, 1.0 - UV.st.y)).rgb;
}
