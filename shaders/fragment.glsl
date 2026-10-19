#version 330 core
in vec3 Normal;
in vec3 FragPos;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
out vec4 FragColor;
void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * objectColor;
    vec3 ambient = 0.1 * objectColor;
    FragColor = vec4(ambient + diffuse, 1.0);
}