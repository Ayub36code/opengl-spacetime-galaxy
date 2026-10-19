#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Shader.h"

struct Planet {
    glm::vec3 position;
    float radius;
    glm::vec3 color;
    float angle;
    float speed;
    float orbitRadius;
    unsigned int VAO, VBO, EBO;
    int indexCount;
    
    // Moon data
    bool hasMoon = false;
    float moonAngle = 0;
    float moonSpeed = 5.0f;
    float moonOrbitRadius = 2.0f;
    glm::vec3 moonPosition;
};

class PlanetSystem {
public:
    std::vector<std::unique_ptr<Planet>> planets;
    unsigned int sphereVAO, sphereVBO, sphereEBO;
    int sphereIndexCount;

    PlanetSystem();
    void update(float dt);
    void render(Shader& s);

private:
    void createSphere();
    void addPlanet(float r, float dist, float spd, glm::vec3 col, bool withMoon = false);
};