#include "SpacetimeGrid.h"
#include <vector>

SpacetimeGrid::SpacetimeGrid(float size, int res) {
    std::vector<float> verts;
    std::vector<unsigned int> inds;
    float step = size / res;
    float half = size / 2.0f;

    for (int z = 0; z <= res; z++) {
        for (int x = 0; x <= res; x++) {
            verts.push_back(-half + x * step);
            verts.push_back(0.0f);
            verts.push_back(-half + z * step);
        }
    }
    for (int z = 0; z < res; z++) {
        for (int x = 0; x < res; x++) {
            int tl = z * (res + 1) + x;
            int tr = tl + 1;
            int bl = (z + 1) * (res + 1) + x;
            int br = bl + 1;
            inds.insert(inds.end(), { (unsigned int)tl, (unsigned int)bl, (unsigned int)tr,
                                      (unsigned int)tr, (unsigned int)bl, (unsigned int)br });
        }
    }
    indexCount = inds.size();
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO); glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); glBufferData(GL_ARRAY_BUFFER, verts.size() * 4, verts.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * 4, inds.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0);
    glEnableVertexAttribArray(0);
}

void SpacetimeGrid::render(Shader& s) {
    // Removed GL_LINE. We now draw a solid surface.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    s.setMat4("model", glm::mat4(1.0f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}