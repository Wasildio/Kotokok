// Scene.h
#pragma once

#include <vector>
#include <memory>
#include "Particle.h"
#include "Figure.h"

class Scene {
private:
    std::vector<Particle> particles;
    std::shared_ptr<Figure> boundary;
    float particleRadius;

public:
    Scene(std::shared_ptr<Figure> fig, float radius = 0.5f);

    void updateAll(float dt);
    void addParticleRandom();

    const std::vector<Particle>& getParticles() const;
};

