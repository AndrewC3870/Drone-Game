#version 330

in vec2 texcoord;

uniform sampler2D texture_1;
uniform sampler2D texture_2;

layout(location = 0) out vec4 out_color;

void main()
{
    vec4 color1 = texture(texture_1, texcoord);
    vec4 color2 = texture(texture_2, texcoord);

    float noiseValue = texture(texture_1, texcoord).r;
    vec4 blendedColor = mix(color1, color2, noiseValue);

    out_color = blendedColor;

    
    if (out_color.a < 0.5f) {
        discard;
    }
}
