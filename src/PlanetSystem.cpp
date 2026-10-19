#include "PlanetSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>

PlanetSystem::PlanetSystem() {
    createSphere();

    // Distances and speeds are scaled for visual appeal
    // Colors are approximated to real-life planetary colors

    // Mercury (Grayish)
    addPlanet(0.4f, 8.0f, 4.0f, glm::vec3(0.6f, 0.6f, 0.6f));

    // Venus (Yellowish/Orange)
    addPlanet(0.6f, 12.0f, 2.0f, glm::vec3(0.9f, 0.7f, 0.3f));

    // Earth (Blue) + Moon
    addPlanet(0.65f, 16.0f, 1.5f, glm::vec3(0.2f, 0.5f, 1.0f), true);

    // Mars (Red)
    addPlanet(0.5f, 21.0f, 1.2f, glm::vec3(0.8f, 0.2f, 0.1f));

    // Jupiter (Gas Giant - Tan/Orange)
    addPlanet(1.6f, 30.0f, 0.6f, glm::vec3(0.8f, 0.6f, 0.4f));

    // Saturn (Pale Gold)
    addPlanet(1.4f, 38.0f, 0.4f, glm::vec3(0.9f, 0.8f, 0.5f));

    // Uranus (Cyan)
    addPlanet(1.0f, 46.0f, 0.3f, glm::vec3(0.5f, 0.8f, 0.9f));

    // Neptune (Deep Blue)
    addPlanet(0.9f, 54.0f, 0.2f, glm::vec3(0.2f, 0.3f, 0.8f));
}

void PlanetSystem::addPlanet(float r, float dist, float spd, glm::vec3 col, bool withMoon) {
    auto p = std::make_unique<Planet>();
    p->radius = r;
    p->orbitRadius = dist;
    p->speed = spd;
    p->color = col;
    p->angle = rand() % 360; // Random starting position

    if (withMoon) {
        p->hasMoon = true;
        p->moonOrbitRadius = r + 1.2f;
    }

    planets.push_back(std::move(p));
}

void PlanetSystem::update(float dt) {
    for (auto& p : planets) {
        // Update Planet Orbit
        p->angle += p->speed * dt * 0.2f;
        p->position = glm::vec3(cos(p->angle) * p->orbitRadius, 0, sin(p->angle) * p->orbitRadius);

        // Update Moon Orbit
        if (p->hasMoon) {
            p->moonAngle += p->moonSpeed * dt;
            p->moonPosition = p->position + glm::vec3(
                cos(p->moonAngle) * p->moonOrbitRadius,
                0,
                sin(p->moonAngle) * p->moonOrbitRadius
            );
        }
    }
}

void PlanetSystem::render(Shader& s) {
    // 1. Render Sun (Scale 3.5, bright yellow-orange)
    glm::mat4 m = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f));
    s.setMat4("model", m);
    s.setVec3("objectColor", glm::vec3(1.0f, 0.8f, 0.2f));
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

    // 2. Render Planets
    for (auto& p : planets) {
        // Render Planet
        m = glm::translate(glm::mat4(1.0f), p->position);
        m = glm::scale(m, glm::vec3(p->radius));
        s.setMat4("model", m);
        s.setVec3("objectColor", p->color);
        glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

        // Render Moon (if planet has one)
        if (p->hasMoon) {
            glm::mat4 moonM = glm::translate(glm::mat4(1.0f), p->moonPosition);
            moonM = glm::scale(moonM, glm::vec3(0.2f)); // Moon is smaller
            s.setMat4("model", moonM);
            s.setVec3("objectColor", glm::vec3(0.8f, 0.8f, 0.8f)); // Gray
            glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
        }
    }
}

// Creates a single high-res sphere for ALL objects to share
void PlanetSystem::createSphere() {
    std::vector<float> v; std::vector<unsigned int> i;
    int rings = 32, sectors = 32;
    for(int r = 0; r <= rings; ++r) {
        for(int st = 0; st <= sectors; ++st) {
            float x = sin(glm::radians(r * 180.0f / rings)) * cos(glm::radians(st * 360.0f / sectors));
            float y = cos(glm::radians(r * 180.0f / rings));
            float z = sin(glm::radians(r * 180.0f / rings)) * sin(glm::radians(st * 360.0f / sectors));
            v.insert(v.end(), {x, y, z, x, y, z});
        }
    }
    for(int r = 0; r < rings; ++r) {
        for(int st = 0; st < sectors; ++st) {
            int a = r * (sectors + 1) + st;
            int b = a + sectors + 1;
            i.insert(i.end(), { (unsigned int)a, (unsigned int)b, (unsigned int)a + 1,
                                (unsigned int)a + 1, (unsigned int)b, (unsigned int)b + 1 });
        }
    }
    sphereIndexCount = i.size();
    glGenVertexArrays(1, &sphereVAO); glGenBuffers(1, &sphereVBO); glGenBuffers(1, &sphereEBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO); glBufferData(GL_ARRAY_BUFFER, v.size() * 4, v.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * 4, i.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * 4, (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * 4, (void*)(3 * 4)); glEnableVertexAttribArray(1);
}