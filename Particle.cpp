#include "Particle.h"
#include <cmath>

Particle::Particle(const glm::vec2& pos, const glm::vec2& vel, float r, float m, const glm::vec3& c)
    : position(pos), velocity(vel), radius(r), mass(m), color(c) {}

void Particle::update(float dt) {
    position += velocity * dt;
}

