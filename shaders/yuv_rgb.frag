#version 330 core

uniform sampler2D tex_y;
// uniform sampler2D tex_u;
// uniform sampler2D tex_v;

in vec2 TexCoord;

out vec4 FragColor;

void main() {
    float y = texture(tex_y, TexCoord).r;
    // float u = texture(tex_u, TexCoord).r / 255 - 0.5f;
    // float v = texture(tex_v, TexCoord).r / 255 - 0.5f;

    // float r = y + 1.402f * v;
    // float g = y - 0.344f * u - 0.714f * v;
    // float b = y + 1.772 * u;

    // FragColor = vec4(r, g, b, 1.0f);

    FragColor(tex_y, 0.0f, 0.0f, 1.0f);
}