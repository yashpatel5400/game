/*******************************************************************************
 * Filename     :   sdf.fs
 * Content      :   Fullscreen SDF FS
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel
 * Language     :   GLSL
*******************************************************************************/

#version 420 core
layout (location = 0) in vec2 aPos;

uniform float time;

out vec4 FragColor;

in vec2 screenPos;

struct Ball
{
    float radius;
    vec3 position;
    vec3 color;
};

struct Light
{
    float intensity;
    vec3 position;
    vec3 color;
};

bool insideSphere(Ball ball, vec3 pos) {
    if (length(ball.position - pos) < ball.radius) {
        return true;
	}
    return false;
}

void main()
{
    // assume right handed coord system, so X/Y are left/right and z into the screen
    Light light = Light(0.50, vec3(1.0, 1.0, -2.5), vec3(1.0)); // white light
    Ball ball = Ball(1.0, vec3(0.0, 0.0, -2.5), vec3(1.0, 0.0, 0.0));

    float bounceFreq = 2.0;
    ball.position.y = abs(sin(bounceFreq * time));

    // we assume the rays are being cast from the origin, so ray point is dir * t
    vec3 pos = vec3(screenPos, -1.0);
    vec3 dir = normalize(pos);
    const float kStepSize = 0.05;
    const int kNumSteps = 20;

    for (int step = 0; step < kNumSteps; step++) {
        pos += dir * kStepSize;
        if (insideSphere(ball, pos)) {
            float lightStrength = light.intensity / length(pos - light.position);
            FragColor = vec4(ball.color * lightStrength, 1.0);
            return;  
		}
	}

    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}