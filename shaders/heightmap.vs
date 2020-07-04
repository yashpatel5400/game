/*******************************************************************************
 * Filename     :   sdf.vs
 * Content      :   Fullscreen SDF VS
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
	gl_Position = projection * view * vec4(pos, 1);
}