// Scene.cpp

#include "Scene.h"
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

Scene::Scene(std::shared_ptr<Figure> fig, float radius)
    : boundary(std::move(fig)), particleRadius(radius) {}

void Scene::updateAll(float dt) {
    size_t n = particles.size();
    if (n < 2) {
        for (auto& p : particles) {
            p.update(dt);
            boundary->reflect(
                const_cast<glm::vec2&>(p.getPosition()),
                const_cast<glm::vec2&>(p.getVelocity()),
                p.getRadius()
            );
        }
        return;
    }

    for (auto& p : particles) {
        p.update(dt);
        boundary->reflect(
            const_cast<glm::vec2&>(p.getPosition()),
            const_cast<glm::vec2&>(p.getVelocity()),
            p.getRadius()
        );
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            glm::vec2 pi = particles[i].getPosition();
            glm::vec2 pj = particles[j].getPosition();
            float r1 = particles[i].getRadius();
            float r2 = particles[j].getRadius();
            glm::vec2 delta = pj - pi;

            float minDist = r1 + r2;
            float dist2 = glm::dot(delta, delta);
            if (dist2 >= minDist * minDist || dist2 == 0.0f) 
                continue;

            float dist = std::sqrt(dist2);
            glm::vec2 nrm = delta / dist;

            glm::vec2 v1 = particles[i].getVelocity();
            glm::vec2 v2 = particles[j].getVelocity();

            glm::vec2 v1r = v1 - 2.0f * glm::dot(v1, nrm) * nrm;
            glm::vec2 v2r = v2 - 2.0f * glm::dot(v2, nrm) * nrm;

            particles[i].setVelocity(v1r);
            particles[j].setVelocity(v2r);

            float overlap = 0.5f * (minDist - dist);
            particles[i].setPosition(pi - overlap * nrm);
            particles[j].setPosition(pj + overlap * nrm);
        }
    }
}

void Scene::addParticleRandom() {
    if (particles.size() >= 100) return;

    glm::vec2 pos;
    bool valid = false;
    while (!valid) {
        pos = glm::linearRand(
            glm::vec2(-25.0f + particleRadius, -25.0f + particleRadius),
            glm::vec2( 25.0f - particleRadius,  25.0f - particleRadius)
        );
        valid = true;
        for (const auto& p : particles) {
            if (glm::length(pos - p.getPosition()) < 2.0f * particleRadius) {
                valid = false;
                break;
            }
        }
        if (!boundary->isInside(pos)) {
            valid = false;
        }
    }

    float angle = glm::linearRand(0.0f, glm::two_pi<float>());
    glm::vec2 vel = 2.0f * glm::vec2(std::cos(angle), std::sin(angle));

    particles.emplace_back(pos, vel, particleRadius, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
}


const std::vector<Particle>& Scene::getParticles() const {
    return particles;
}

