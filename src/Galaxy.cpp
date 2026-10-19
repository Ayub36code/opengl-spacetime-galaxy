#include "Galaxy.h"
#include <random>

#include "glad.h"

Galaxy::Galaxy(int c) : count(c) {
    std::vector<float> data;
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    for(int i=0; i<count; i++) {
        float r = 3500.0f * sqrt(dis(gen));
        float theta = (r / 3500.0f) * 5.0f + (dis(gen) - 0.5f) * 1.5f;
        float x = r * cos(theta), z = r * sin(theta);
        float y = (dis(gen) - 0.5f) * (200.0f * (1.0f - r/3500.0f));
        data.insert(data.end(), {x, y, z, 0.8f, 0.9f, 1.0f, 2.0f});
    }
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); glBufferData(GL_ARRAY_BUFFER, data.size()*4, data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*4, (void*)0); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*4, (void*)(3*4)); glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7*4, (void*)(6*4)); glEnableVertexAttribArray(2);
}

void Galaxy::render(Shader& s) {
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, count);
}