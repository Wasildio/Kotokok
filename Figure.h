// Figure.h
#pragma once

#include <glm/glm.hpp>

class Figure {
public:
    virtual ~Figure() = default;

    // Проверка: находится ли точка внутри фигуры
    virtual bool isInside(const glm::vec2& pos) const = 0;

    // Отразить скорость частицы при столкновении со стенкой
    virtual void reflect(glm::vec2& pos, glm::vec2& vel, float radius) const = 0;

    // Рендер фигуры (опционально)
    virtual void render() const = 0;
};

