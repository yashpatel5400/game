/*******************************************************************************
 * Filename     :   sdf.vs
 * Content      :   Fullscreen SDF VS
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel
 * Language     :   GLSL
*******************************************************************************/

#version 420 core
layout (location = 0) in vec2 aPos;

out vec2 pos;

void main()
{
	gl_Position = vec4(vec2(2) * aPos - vec2(1), 0, 1);
    pos = gl_Position.xy;
}