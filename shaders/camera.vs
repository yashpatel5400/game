/*******************************************************************************
 * Filename     :   camera.vs
 * Content      :   Basic camera vertex shader
 * Created      :   June 21, 2020
 * Authors      :   Joey de Vries (https://learnopengl.com/Getting-started/Textures)
 * Language     :   GLSL
*******************************************************************************/

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}