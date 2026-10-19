#ifndef OPENGL_SPACETIME_GALAXY_SPACETIMEGRID_H
#define OPENGL_SPACETIME_GALAXY_SPACETIMEGRID_H

#include "Shader.h"
class SpacetimeGrid {

    public:
        unsigned int VAO, VBO, EBO;
        int indexCount;
        SpacetimeGrid(float size, int res);

        void render(Shader& s);
};


#endif //OPENGL_SPACETIME_GALAXY_SPACETIMEGRID_H
