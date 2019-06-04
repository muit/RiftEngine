## VERTEX ##

#version 330 core

const float PI = 3.14159265359;

// Angle from 0 to n, where 360 is 2PI
layout (location = 0) in float angle;

uniform mat4 mvp;

out vec3 normal;
out vec2 uv;

void main()
{
    float distance = angle / 2 * PI * 0.3f; // 1 loop = 1 distance. Also adds a multiplier

	vec4 pos = vec4(distance * cos(angle), distance * sin(angle), 0.0, 1.0);
    //vec4 pos = vec4(distance, 0.0, 0.0, 1.0);
    gl_Position = mvp * pos;
}


## FRAGMENT ##

#version 330 core


void main()
{
    vec3 color = vec3(0.1, 0.9, 0.1);

	gl_FragColor = vec4(color, 1.0);
}
