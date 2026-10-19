#pragma once
#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
public:
    unsigned int ID;
    Shader(const char* vPath, const char* fPath);
    void use() const;

    // Notice the 'const' at the end of these definitions
    void setFloat(const std::string& n, float v) const;
    void setInt(const std::string& n, int v) const;
    void setVec3(const std::string& n, const glm::vec3& v) const;
    void setMat4(const std::string& n, const glm::mat4& m) const;
};