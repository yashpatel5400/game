/*******************************************************************************
 * Filename     :   camera.fs
 * Content      :   Basic camera fragment shader
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel, Joey de Vries (https://learnopengl.com/Getting-started/Textures)
 * Language     :   GLSL
*******************************************************************************/

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture;

void main()
{
	FragColor = texture(texture, TexCoord);
}