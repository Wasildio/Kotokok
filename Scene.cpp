#include "Scene.h"
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>  // для glm::two_pi<float>()
#include <cmath>
#include <vector>

Scene::Scene(std::shared_ptr<Figure> fig, float radius)
    : boundary(std::move(fig)), particleRadius(radius) {}
void Scene::updateAll(float dt) {
    // 1) Обновляем каждую частицу по dt и отражаем от границ:
    for (auto& p : particles) {
        // обновление позиции
        p.update(dt);
        // отражение от стенок квадрата
        boundary->reflect(
            /* pos    */ const_cast<glm::vec2&>(p.getPosition()),
            /* vel    */ const_cast<glm::vec2&>(p.getVelocity()),
            /* radius */ p.getRadius()
        );
    }

    // 2) Обработка парных столкновений:
    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            // позиции и вектор между ними
            glm::vec2 pi = particles[i].getPosition();
            glm::vec2 pj = particles[j].getPosition();
            glm::vec2 delta = pj - pi;

            float r1 = particles[i].getRadius();
            float r2 = particles[j].getRadius();
            float minDist = r1 + r2;
            float dist2   = glm::dot(delta, delta);

            // проверка перекрытия
            if (dist2 < minDist * minDist && dist2 > 0.0f) {
                float distance = std::sqrt(dist2);
                glm::vec2 normal = delta / distance;

                // относительная скорость
                glm::vec2 vi = particles[i].getVelocity();
                glm::vec2 vj = particles[j].getVelocity();
                glm::vec2 relVel = vi - vj;

                float velAlongNormal = glm::dot(relVel, normal);
                if (velAlongNormal >= 0.0f)
                    continue; // частицы уже расходятся

                // рассчитываем упругий импульс
                float m1 = particles[i].getMass();
                float m2 = particles[j].getMass();
                float jImpulse = -(2.0f * velAlongNormal) / (m1 + m2);

                // обновляем скорости
                particles[i].setVelocity(vi + (jImpulse * m2) * normal);
                particles[j].setVelocity(vj - (jImpulse * m1) * normal);

                // раздвигаем частицы, чтобы убрать перекрытие
                float overlap = 0.5f * (minDist - distance);
                glm::vec2 correction = normal * overlap;

                particles[i].setPosition(pi - correction);
                particles[j].setPosition(pj + correction);
            }
        }
    }
}

void Scene::addParticleRandom() {
    if (particles.size() >= 50) return;

    glm::vec2 pos;
    bool valid = false;
    while (!valid) {
        pos = glm::linearRand(
            glm::vec2(-24.5f, -24.5f),
            glm::vec2( 24.5f,  24.5f)
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
    glm::vec2 vel = 2.0f * glm::vec2(glm::cos(angle), glm::sin(angle));

    float mass = 1.0f;
    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

    particles.emplace_back(pos, vel, particleRadius, mass, color);
}

const std::vector<Particle>& Scene::getParticles() const {
    return particles;
}
