#version 330 core

uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;

in vec2 TexCoord;

out vec4 FragColor;

// see: https://zhuanlan.zhihu.com/p/436186749

void main() {
    float y = texture(tex_y, TexCoord).r - (16.0f / 255.0f);
    // float y = texture(tex_y, TexCoord).r;
    float u = texture(tex_u, TexCoord).r - 0.5f;
    float v = texture(tex_v, TexCoord).r - 0.5f;
    // BT601
    // float r = y + 1.402f * v;
    // float g = y - 0.34414f * u - 0.71414f * v;
    // float b = y + 1.772 * u;

    // BT709
    float r = 1.164f * y + 1.793f * v;
    float g = 1.164f * y - 0.213f * u - 0.533f * v;
    float b = 1.164f * y + 2.112f * u;

    // float a = texture(tex_v, TexCoord).r;
    FragColor = vec4(r, g, b, 1.0f);

    // FragColor = vec4(y, 0.0f, 0.0f, 1.0f);
}