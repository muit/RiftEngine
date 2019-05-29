## VERTEX ##

#version 330 core
layout (location = 0) in vec3 v_position;

uniform mat4 vp;

out vec3 uv;

void main()
{
    uv = vec3(v_position.x, -v_position.y, v_position.z);
    gl_Position = vp * vec4(v_position, 1.0);
} 


## FRAGMENT ##

#version 330 core

in vec3 uv;
out vec4 fragment_color;

uniform samplerCube skybox;

void main()
{    
    fragment_color = texture(skybox, uv);
}
