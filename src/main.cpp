#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

#include "Shader.h"
#include "Camera.h"
#include "SpacetimeGrid.h"
#include "PlanetSystem.h"
#include "Galaxy.h"

static Camera camera;
double lastX = 800, lastY = 450;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float dx = xpos - lastX;
    float dy = lastY - ypos;
    lastX = xpos; lastY = ypos;
    if (camera.leftDrag) {
        camera.targetYaw += dx * 0.4f;
        camera.targetPitch = glm::clamp(camera.targetPitch + dy * 0.4f, -85.0f, 85.0f);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Exponential zooming based on scroll
    camera.targetDistance *= pow(0.9f, (float)yoffset);
    camera.targetDistance = glm::clamp(camera.targetDistance, camera.minDistance, camera.maxDistance);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) camera.leftDrag = (action == GLFW_PRESS);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Spacetime & Galaxy", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Load Shaders
    Shader planetShader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Shader gridShader("shaders/spacetime_vertex.glsl", "shaders/spacetime_fragment.glsl");
    Shader galaxyShader("shaders/galaxy_vertex.glsl", "shaders/galaxy_fragment.glsl");

    // Init Objects
    SpacetimeGrid grid(2000.0f, 150);
    PlanetSystem solarSystem;
    Galaxy galaxy(50000);

    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();
        float dt = 0.016f; // Fixed delta for simplicity
        
        camera.aspectRatio = 1600.0f / 900.0f;
        camera.update(dt);
        
        solarSystem.update(dt);

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = camera.getProjection();
        glm::mat4 view = camera.getView();

        // 1. Calculate Zoom Visibility
        float camDist = camera.distance;
        float galaxyVis = glm::clamp((camDist - 200.0f) / 400.0f, 0.0f, 1.0f);
        float gridVis = 1.0f - galaxyVis; // Grid fades out as galaxy fades in

        // 2. Render Galaxy (If zoomed out)
        if (galaxyVis > 0.01f) {
            glDisable(GL_DEPTH_TEST); // Galaxy is backdrop
            galaxyShader.use();
            galaxyShader.setMat4("projection", proj);
            galaxyShader.setMat4("view", view);
            galaxyShader.setFloat("galaxyTime", time);
            galaxyShader.setFloat("galaxyVisibility", galaxyVis);
            galaxy.render(galaxyShader);
            glEnable(GL_DEPTH_TEST);
        }

        // 3. Render Spacetime Grid (If zoomed in)
        // Inside main.cpp's render loop:

        // 3. Render Spacetime Grid
        if (gridVis > 0.01f) {
            gridShader.use();
            gridShader.setMat4("projection", proj);
            gridShader.setMat4("view", view);

            // Upload Body Physics (Sun + All 8 Planets)
            int numBodies = 1 + solarSystem.planets.size();
            gridShader.setInt("numBodies", numBodies);

            // The Sun (Mass is huge to create the central deep well)
            gridShader.setVec3("bodyPositions[0]", glm::vec3(0.0f));
            gridShader.setFloat("bodyMasses[0]", 40.0f);

            // The Planets
            for(int i=0; i<solarSystem.planets.size(); i++) {
                std::string posU = "bodyPositions[" + std::to_string(i+1) + "]";
                std::string massU = "bodyMasses[" + std::to_string(i+1) + "]";
                gridShader.setVec3(posU, solarSystem.planets[i]->position);
                // Mass is proportional to radius (larger planets make deeper wells)
                gridShader.setFloat(massU, solarSystem.planets[i]->radius * 4.0f);
            }

            grid.render(gridShader);
        }


        // 4. Render Solar System (Planets)
        // (They scale down visually if we scale the camera, but we only see them up close)
        if (gridVis > 0.1f) {
            planetShader.use();
            planetShader.setMat4("projection", proj);
            planetShader.setMat4("view", view);
            planetShader.setVec3("viewPos", camera.target + glm::vec3(0,0,camera.distance)); // approx
            planetShader.setVec3("lightPos", glm::vec3(0.0f));
            solarSystem.render(planetShader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}