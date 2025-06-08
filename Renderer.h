#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // Для glm::ortho
#include "Scene.h"
#include <string>

class Renderer {
private:
    GLuint particleVAO, particleVBO;
    GLuint squareVAO, squareVBO;

    GLuint particleShader;
    GLuint squareShader;

    glm::mat4 projection; // <--- Новое поле

public:
    Renderer();
    ~Renderer();

    void init();
    void render(const Scene& scene) const;

private:
    GLuint compileShader(const std::string& vertexPath, const std::string& fragmentPath) const;
};
