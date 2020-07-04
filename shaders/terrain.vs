/*******************************************************************************
 * Filename     :   terrain.vs
 * Content      :   Random terrain generation VS
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel
 * Language     :   GLSL
*******************************************************************************/

#version 420 core
layout (location = 0) in vec3 aPos;

uniform float time;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 pos = aPos;

    const float amplitude = 0.25;
    const float freq = 10.0;
    const float velocity = 3.0;
    pos.y = amplitude * sin(pos.z * freq + velocity * time);
	gl_Position = projection * view * vec4(pos, 1);
}