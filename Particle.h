#pragma once

#include <glm/glm.hpp>

class Particle {
private:
    glm::vec2 position;
    glm::vec2 velocity;
    float radius;
    float mass;
    glm::vec3 color;

public:
    Particle(const glm::vec2& pos, const glm::vec2& vel, float r, float m, const glm::vec3& c);

    void update(float dt);

    const glm::vec2& getPosition() const { return position; }
    const glm::vec2& getVelocity() const { return velocity; }
    float getRadius() const         { return radius;   }
    float getMass() const           { return mass;     }
    const glm::vec3& getColor() const { return color;   }

    void setPosition(const glm::vec2& p) { position = p; }
    void setVelocity(const glm::vec2& v) { velocity = v; }
};
