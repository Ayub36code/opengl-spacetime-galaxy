#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vPath, const char* fPath) {
    std::string vCode, fCode;
    std::ifstream vFile(vPath), fFile(fPath);
    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf(); vCode = vStream.str();
    fStream << fFile.rdbuf(); fCode = fStream.str();

    unsigned int v = glCreateShader(GL_VERTEX_SHADER);
    const char* vSrc = vCode.c_str();
    glShaderSource(v, 1, &vSrc, NULL); glCompileShader(v);

    unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSrc = fCode.c_str();
    glShaderSource(f, 1, &fSrc, NULL); glCompileShader(f);

    ID = glCreateProgram();
    glAttachShader(ID, v); glAttachShader(ID, f); glLinkProgram(ID);
    glDeleteShader(v); glDeleteShader(f);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setFloat(const std::string& n, float v) const {
    glUniform1f(glGetUniformLocation(ID, n.c_str()), v);
}

void Shader::setInt(const std::string& n, int v) const {
    glUniform1i(glGetUniformLocation(ID, n.c_str()), v);
}

void Shader::setVec3(const std::string& n, const glm::vec3& v) const {
    glUniform3fv(glGetUniformLocation(ID, n.c_str()), 1, glm::value_ptr(v));
}

void Shader::setMat4(const std::string& n, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, n.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}