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

struct Ray {
    vec3 point;
    vec3 direction; // assumed normalized
};

struct Camera {
    vec3 position;

    // camera coordinate system
    // note that any one can be determined from the other two as norm(cross prod)
    vec3 up;
    vec3 right;
    vec3 forward;
};

bool insideSphere(Ball ball, vec3 pos) {
    if (length(ball.position - pos) < ball.radius) {
        return true;
	}
    return false;
}

vec3 cameraToWorld(Camera camera, vec3 coord) {
    mat3 transform = mat3(camera.right, camera.up, -camera.forward);
    return camera.position + transform * coord;
}

void main()
{
    // assume right handed coord system, so X/Y are left/right and z into the screen
    Light light = Light(0.50, vec3(1.0, 1.0, 0.0), vec3(1.0)); // white light
    Ball ball = Ball(1.0, vec3(0.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0));

    float bounceFreq = 2.0;
    ball.position.y += abs(sin(bounceFreq * time));

    // we assume the rays are being cast from the origin, so ray point is dir * t
    vec3 origin = vec3(0.0);
    vec3 cameraPos = vec3(5.0 * sin(time), 1.0, 5.0 * cos(time));
    vec3 cameraUp = vec3(0.0, 1.0, 0.0); // assume camera on x-z plane
    vec3 cameraForward = normalize(origin - cameraPos); // camera faces origin
    vec3 cameraRight = normalize(cross(cameraForward, cameraUp));
    Camera camera = Camera(cameraPos, cameraUp, cameraRight, cameraForward);
    
    vec3 dir = normalize(vec3(screenPos, -1.0));
    vec3 cameraRay = origin; // we track this in camera space and then transform
    
    const float kStepSize = .10;
    const int kNumSteps = 100;
    for (int step = 0; step < kNumSteps; step++) {
        cameraRay += dir * kStepSize;
        vec3 worldRay = cameraToWorld(camera, cameraRay);
        
        // show the light source
        if (length(light.position - worldRay) < 0.10) {
            FragColor = vec4(1.0);
            return;  
		}

        // character
        if (insideSphere(ball, worldRay)) {
            float lightStrength = light.intensity / length(worldRay - light.position);
            FragColor = vec4(ball.color * lightStrength, 1.0);
            return;  
		}

        // floor
        if (worldRay.y < 0.0) {
            float lightStrength = light.intensity / length(worldRay - light.position);
            FragColor = vec4(vec3(0.0, 1.0, 0.0) * lightStrength, 1.0);
            return;  
		}
	}

    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}