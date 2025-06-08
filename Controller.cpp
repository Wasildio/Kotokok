// Controller.cpp
#include "Controller.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Square.h"
#include <iostream>

Controller::Controller(int width, int height)
    : windowWidth(width), windowHeight(height), window(nullptr) {
    if (!initWindow()) {
        std::cerr << "Failed to initialize window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, windowWidth, windowHeight);

    auto square = std::make_shared<Square>(-25.0f, 25.0f, -25.0f, 25.0f);
    scene = std::make_shared<Scene>(square);

    renderer = std::make_unique<Renderer>();

    // Изначально добавим несколько частиц
    for (int i = 0; i < 50; ++i) {
        scene->addParticleRandom();
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
    float lastTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput();
        scene->updateAll(deltaTime);
        renderer->render(*scene);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

