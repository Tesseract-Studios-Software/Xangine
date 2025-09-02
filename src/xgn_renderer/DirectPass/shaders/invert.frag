#version 430 core

uniform sampler2D inputTexture;

in vec2 vTexCoord;
out vec4 out_FragColor;

void main() {
    vec4 color = texture(inputTexture, vTexCoord);
    out_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);
}