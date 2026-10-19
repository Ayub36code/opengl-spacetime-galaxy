#version 330 core
in float depth;
in vec3 gridColor;
out vec4 FragColor;

void main() {
    // Solid shading with the calculated shadow color
    FragColor = vec4(gridColor, 1.0);
}