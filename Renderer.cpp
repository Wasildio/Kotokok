#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

Renderer::Renderer() {
    particleShader = compileShader("shaders/particle.vert", "shaders/particle.frag");
    squareShader = compileShader("shaders/square.vert", "shaders/square.frag");

    projection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, -1.0f, 1.0f);

    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);

    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);

    constexpr int numSegments = 36;
    constexpr int numVertices = numSegments + 2;
    float vertices[numVertices * 2];

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;

    for (int i = 0; i <= numSegments; ++i) {
        float angle = i * 2.0f * 3.1415926f / numSegments;
        vertices[(i + 1) * 2] = cos(angle);
        vertices[(i + 1) * 2 + 1] = sin(angle);
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &squareVAO);
    glGenBuffers(1, &squareVBO);

    float squareVertices[] = {
        -25.0f, -25.0f,
         25.0f, -25.0f,
         25.0f,  25.0f,
        -25.0f,  25.0f
    };

    glBindVertexArray(squareVAO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &particleVAO);
    glDeleteBuffers(1, &particleVBO);
    glDeleteVertexArrays(1, &squareVAO);
    glDeleteBuffers(1, &squareVBO);
}

void Renderer::render(const Scene& scene) const {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(squareShader);

    GLuint projLoc = glGetUniformLocation(squareShader, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(squareVAO);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);

    glUseProgram(particleShader);

    projLoc = glGetUniformLocation(particleShader, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(particleVAO);

    for (const auto& particle : scene.getParticles()) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(particle.getPosition(), 0.0f));
        model = glm::scale(model, glm::vec3(particle.getRadius(), particle.getRadius(), 1.0f));

        GLuint modelLoc = glGetUniformLocation(particleShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

        GLuint colorLoc = glGetUniformLocation(particleShader, "color");
        glUniform3fv(colorLoc, 1, &particle.getColor()[0]);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 38);
    }

    glBindVertexArray(0);
}

GLuint Renderer::compileShader(const std::string& vertexPath, const std::string& fragmentPath) const {
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);

    std::stringstream vertexSStream, fragmentSStream;
    vertexSStream << vertexFile.rdbuf();
    fragmentSStream << fragmentFile.rdbuf();

    std::string vertexCode = vertexSStream.str();
    std::string fragmentCode = fragmentSStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

