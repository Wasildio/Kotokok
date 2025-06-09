#pragma once

#include "Figure.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Square : public Figure {
private:
    float vertices[8];
    GLuint vao, vbo;

    float x_min, x_max, y_min, y_max;

public:
    Square(float x_min, float x_max, float y_min, float y_max);
    ~Square();

    virtual void render() const override;
    virtual bool isInside(const glm::vec2& pos) const override;
    virtual void reflect(glm::vec2& pos, glm::vec2& vel, float radius) const override;
};
