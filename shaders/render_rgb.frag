#version 430 core

in vec2 texCoord;

uniform sampler2D rData;
uniform sampler2D gData;
uniform sampler2D bData;

out vec4 fragColor;

void main() {
    float r = texture(rData, texCoord).x;
    float g = texture(gData, texCoord).x;
    float b = texture(bData, texCoord).x;
    fragColor = vec4(r, g, b, 1.0);
}
