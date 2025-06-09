#include "Controller.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Square.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>
#include <iostream>

Controller::Controller(int width, int height)
    : windowWidth(width), windowHeight(height), window(nullptr)
{
    if (!initWindow()) {
        std::cerr << "Failed to initialize window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, windowWidth, windowHeight);

    constexpr float x_min = -25.0f, x_max = 25.0f;
    constexpr float y_min = -25.0f, y_max = 25.0f;
    auto square = std::make_shared<Square>(x_min, x_max, y_min, y_max);
    scene = std::make_shared<Scene>(square);

    renderer = std::make_unique<Renderer>();

    for (int i = 0; i < 49; ++i) {
        scene->addParticleRandom();
    }

    {
        const float bigR = 8.0f;
        const float bigM = 5.0f;
        const glm::vec3 blue(0.0f, 0.0f, 1.0f);

        glm::vec2 pos;
        bool ok = false;
        while (!ok) {
            pos = glm::linearRand(
                glm::vec2(x_min + bigR, y_min + bigR),
                glm::vec2(x_max - bigR, y_max - bigR)
            );
            ok = true;
            for (const auto& p : scene->getParticles()) {
                if (glm::length(pos - p.getPosition()) < (p.getRadius() + bigR)) {
                    ok = false;
                    break;
                }
            }
        }

        float angle = glm::linearRand(0.0f, glm::two_pi<float>());
        glm::vec2 vel = 2.0f * glm::vec2(std::cos(angle), std::sin(angle));

        scene->addParticle(pos, vel, bigM, bigR, blue);
    }
}

Controller::~Controller() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Controller::initWindow() {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "Brownian Motion", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Controller::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Controller::mainLoop() {
    const int substeps = 15;
    float lastTime = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float frameTime  = currentTime - lastTime;
        lastTime = currentTime;

        processInput();

        float dt = frameTime / static_cast<float>(substeps);
        for (int i = 0; i < substeps; ++i) {
            scene->updateAll(dt);
        }

        renderer->render(*scene);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

