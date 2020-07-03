/*******************************************************************************
 * Filename     :   sdf.fs
 * Content      :   Fullscreen SDF FS
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel
 * Language     :   GLSL
*******************************************************************************/

#version 420 core
layout (location = 0) in vec2 aPos;

out vec4 FragColor;

in vec2 pos;

struct Ball
{
  float radius;
  vec3 position;
};

void main()
{
    Ball ball = Ball(1.0, vec3(0.0, 0.0, 2.0));

    // we assume the rays are being cast from the origin, so ray point is dir * t
    vec3 dir = normalize(vec3(pos, 1.0));

    if (pos.x < 0.0) {
    	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
}