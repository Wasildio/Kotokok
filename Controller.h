#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Scene.h"
#include "Renderer.h"

class Controller {
private:
    int windowWidth;
    int windowHeight;

    GLFWwindow* window;
    std::shared_ptr<Scene> scene;
    std::unique_ptr<Renderer> renderer;

    bool initWindow();
    void processInput();

public:
    Controller(int width, int height);
    ~Controller();

    void mainLoop();
};
