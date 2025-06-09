// Square.cpp

#include "Square.h"
#include <algorithm>  // для std::clamp

Square::Square(float x_min, float x_max, float y_min, float y_max)
    : x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max)
{
    // Прямое заполнение массива вершин
    vertices[0] = x_min; vertices[1] = y_min;
    vertices[2] = x_max; vertices[3] = y_min;
    vertices[4] = x_max; vertices[5] = y_max;
    vertices[6] = x_min; vertices[7] = y_max;

    // Создание VAO/VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

Square::~Square() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Square::render() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);
}

bool Square::isInside(const glm::vec2& pos) const {
    return pos.x >= x_min && pos.x <= x_max
        && pos.y >= y_min && pos.y <= y_max;
}

void Square::reflect(glm::vec2& pos, glm::vec2& vel, float radius) const {
    bool hitX = (pos.x - radius < x_min) || (pos.x + radius > x_max);
    bool hitY = (pos.y - radius < y_min) || (pos.y + radius > y_max);

    // Если одновременно и по X, и по Y — угол
    if (hitX && hitY) {
        pos.x = std::clamp(pos.x, x_min + radius, x_max - radius);
        pos.y = std::clamp(pos.y, y_min + radius, y_max - radius);
        vel = -vel;
        return;
    }

    // Обычное отражение по X
    if (pos.x - radius < x_min) {
        pos.x = x_min + radius;
        vel.x = -vel.x;
    } else if (pos.x + radius > x_max) {
        pos.x = x_max - radius;
        vel.x = -vel.x;
    }

    // Обычное отражение по Y
    if (pos.y - radius < y_min) {
        pos.y = y_min + radius;
        vel.y = -vel.y;
    } else if (pos.y + radius > y_max) {
        pos.y = y_max - radius;
        vel.y = -vel.y;
    }
}

