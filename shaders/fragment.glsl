#version 460 core

in vec3 vertex_position;

out vec4 frag_color;

void main() {
    vec3 color = normalize(vertex_position);
    frag_color = vec4(1.0, 1.0, 1.0, 1.0);
}