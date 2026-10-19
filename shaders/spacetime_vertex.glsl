#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform int numBodies;
uniform vec3 bodyPositions[16]; // Increased to 16 to fit Sun + Planets
uniform float bodyMasses[16];

out float depth;
out vec3 gridColor;

void main() {
    vec3 pos = aPos;
    float displacement = 0.0;

    // Calculate the dip for every planet and the sun
    for (int i = 0; i < numBodies; i++) {
        float dist = length(pos.xz - bodyPositions[i].xz);
        // The mass pulls the grid down (negative Y).
        // The + 1.0 prevents division by zero.
        displacement -= (bodyMasses[i] * 250.0) / (dist + 1.0);
    }

    pos.y = displacement;

    // Base grid color (light gray/blue)
    vec3 surfaceColor = vec3(0.6, 0.65, 0.7);

    // Darken the grid the deeper it goes (creating the shadow)
    float darkness = clamp(-pos.y / 30.0, 0.0, 0.95);
    gridColor = mix(surfaceColor, vec3(0.02, 0.02, 0.05), darkness);

    gl_Position = projection * view * model * vec4(pos, 1.0);
    depth = -pos.y;
}