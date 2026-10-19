#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aSize;
uniform mat4 projection;
uniform mat4 view;
uniform float galaxyTime;
uniform float galaxyVisibility;
out vec3 Color;
out float Visibility;
void main() {
    float angle = galaxyTime * 0.005f;
    float c = cos(angle);
    float s = sin(angle);
    vec3 rotated = vec3(aPos.x * c - aPos.z * s, aPos.y, aPos.x * s + aPos.z * c);

    vec4 mvPos = view * vec4(rotated, 1.0);
    gl_Position = projection * mvPos;
    gl_PointSize = clamp(aSize * (600.0 / -mvPos.z), 0.5, 12.0);

    Color = aColor;
    Visibility = galaxyVisibility;
}