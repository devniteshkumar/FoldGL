#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main() {
    gl_Position = vec4(aPos, 0.0, 0.5);
    v_TexCoord = texCoord;
}
