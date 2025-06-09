#pragma once

#include <glm/glm.hpp>

class Figure {
public:
    virtual ~Figure() = default;

    virtual bool isInside(const glm::vec2& pos) const = 0;

    virtual void reflect(glm::vec2& pos, glm::vec2& vel, float radius) const = 0;

    virtual void render() const = 0;
};

