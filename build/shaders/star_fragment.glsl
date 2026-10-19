#version 330 core
in vec3 Color;
out vec4 FragColor;
void main() {
    vec2 center = gl_PointCoord - vec2(0.5);
    if (length(center) > 0.5) discard;
    float alpha = smoothstep(0.5, 0.0, length(center));
    FragColor = vec4(Color, alpha);
}