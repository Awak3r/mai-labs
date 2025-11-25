#pragma once
#include "figure.h"
#include <initializer_list>
#include <stdexcept>
#include <cmath>
#include <type_traits>

template<typename T>
class Rectangle : public Figure<T> {
    static_assert(std::is_scalar<T>::value, "Template parameter T must be a scalar type");

public:
    Rectangle() = default;
    Rectangle(const std::initializer_list<Point<T>>& init) : Figure<T>(init) {
        if (init.size() != 4) {
            throw std::logic_error("Rectangle must have exactly 4 vertices");
        }
    }
    virtual operator double() const override {
        if (this->data.size() != 4) {
            return 0.0;
        }
        double S = 0;
        for (size_t i = 0; i < 4; i++) {
            size_t j = (i + 1) % 4;
            S += (this->data[i]->get_x() * this->data[j]->get_y() - 
                  this->data[j]->get_x() * this->data[i]->get_y());
        }
        return std::abs(S) / 2.0;
    }
    virtual ~Rectangle() noexcept override = default;
};

template<typename T>
class Trapezoid : public Figure<T> {
    static_assert(std::is_scalar<T>::value, "Template parameter T must be a scalar type");
    
public:
    Trapezoid() = default;
    Trapezoid(const std::initializer_list<Point<T>>& init) : Figure<T>(init) {
        if (init.size() != 4) {
            throw std::logic_error("Trapezoid must have exactly 4 vertices");
        }
    }
    virtual operator double() const override {
        if (this->data.size() != 4) {
            return 0.0;
        }
        double S = 0;
        for (size_t i = 0; i < 4; i++) {
            size_t j = (i + 1) % 4;
            S += (this->data[i]->get_x() * this->data[j]->get_y() - 
                  this->data[j]->get_x() * this->data[i]->get_y());
        }
        return std::abs(S) / 2.0;
    }
    virtual ~Trapezoid() noexcept override = default;
};

template<typename T>
class Rhombus : public Figure<T> {
    static_assert(std::is_scalar<T>::value, "Template parameter T must be a scalar type");
    
public:
    Rhombus() = default;
    Rhombus(const std::initializer_list<Point<T>>& init) : Figure<T>(init) {
        if (init.size() != 4) {
            throw std::logic_error("Rhombus must have exactly 4 vertices");
        }
    }
    virtual operator double() const override {
        if (this->data.size() != 4) {
            return 0.0;
        }
        double d1 = std::sqrt(
            std::pow(static_cast<double>(this->data[2]->get_x() - this->data[0]->get_x()), 2) + 
            std::pow(static_cast<double>(this->data[2]->get_y() - this->data[0]->get_y()), 2)
        );
        double d2 = std::sqrt(
            std::pow(static_cast<double>(this->data[3]->get_x() - this->data[1]->get_x()), 2) + 
            std::pow(static_cast<double>(this->data[3]->get_y() - this->data[1]->get_y()), 2)
        );
        return (d1 * d2) / 2.0;
    }
    virtual ~Rhombus() noexcept override = default;
};