#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float noiseFrequency;
uniform float noiseAmplitude;

out vec2 texcoord;

float noise(vec2 pos) {
    return fract(sin(dot(pos, vec2(15, 60))) * 50000);
}

void main()
{
    vec3 newPosition = v_position;
    float noiseValue = noise(vec2(newPosition.x, newPosition.z) * noiseFrequency);
    newPosition.y += (noiseValue * 2.0 - 1.0) * noiseAmplitude; 

    texcoord = v_texture_coord;
    gl_Position = Projection * View * Model * vec4(newPosition, 1.0);
}
