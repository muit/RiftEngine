## VERTEX ##

#version 330 core
layout (location = 0) in vec3 v_position;

uniform mat4 vp;

out vec3 uv;

void main()
{
    uv = v_position;
    gl_Position = vp * vec4(v_position, 1.0);
} 


## FRAGMENT ##

#version 330 core

in vec3 uv;

uniform samplerCube skybox;

void main()
{    
    gl_FragColor = texture(skybox, uv);
}
