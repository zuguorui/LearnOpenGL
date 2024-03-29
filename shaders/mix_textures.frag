#version 330 core

uniform sampler2D texture0;
uniform sampler2D texture1;

in vec2 TexCoord;

out vec4 fragColor;

void main() {
    fragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);
}