/*******************************************************************************
 * Filename     :   terrain.fs
 * Content      :   Random bouncers FS
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel
 * Language     :   GLSL
*******************************************************************************/

#version 420 core
layout (location = 0) in vec2 aPos;

uniform float time;

in vec3 pos;

out vec4 FragColor;

// pulled from https://thebookofshaders.com/10/
float random (float s) {
    return fract(sin(s * 437.5453123));
}

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main()
{
    const float kBandWidth = .025;
    float band = float(int(pos.x / kBandWidth));
    float speed = random(band);

    const float kSpotWidth = 0.1;
    float spot = 2.0 * abs(sin(speed * time));

    if (spot < pos.z && pos.z < spot + kSpotWidth) {
        FragColor = vec4(1.0);
	} else {
        FragColor = vec4(0.0);
	}
}