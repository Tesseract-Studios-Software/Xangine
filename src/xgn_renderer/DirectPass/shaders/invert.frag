#version 120

uniform sampler2D inputTexture;
varying vec2 fragTexCoord;

void main() {
    vec4 color = texture2D(inputTexture, fragTexCoord);
    gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);
}