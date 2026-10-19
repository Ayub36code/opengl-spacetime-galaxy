//
// Created by ayub on 10/18/26.
//

#ifndef OPENGL_SPACETIME_GALAXY_GALAXY_H
#define OPENGL_SPACETIME_GALAXY_GALAXY_H
#include "Shader.h"


class Galaxy {
    public:
    unsigned int VAO, VBO;
    int count;

    Galaxy(int c);

    void render(Shader& s);
};


#endif //OPENGL_SPACETIME_GALAXY_GALAXY_H
