#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>

template<typename T>
class Point {
    static_assert(std::is_scalar<T>::value, "Template parameter T must be a scalar type");
    
public:
    Point() : x(0), y(0) {}
    Point(T x_val, T y_val) : x(x_val), y(y_val) {}
    Point(const std::initializer_list<T>& init) {
        auto it = init.begin();
        x = (it != init.end()) ? *it++ : T(0);
        y = (it != init.end()) ? *it : T(0);
    }
    Point(const Point& other) : x(other.x), y(other.y) {}
    Point& operator=(const Point& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    T get_x() const { return x; }
    T get_y() const { return y; }
    void set_x(T x_val) { x = x_val; }
    void set_y(T y_val) { y = y_val; }
    virtual ~Point() noexcept = default;

private:
    T x;
    T y;
};

template<typename T>
class Figure {
    static_assert(std::is_scalar<T>::value, "Template parameter T must be a scalar type");
public:
    Figure() = default;
    Figure(const std::initializer_list<Point<T>>& init) {
        for (const auto& point : init) {
            data.push_back(std::make_unique<Point<T>>(point));
        }
    }
    Figure(const Figure& other) {
        for (const auto& point_ptr : other.data) {
            if (point_ptr) {
                data.push_back(std::make_unique<Point<T>>(*point_ptr));
            }
        }
    }
    Figure(Figure&& other) noexcept = default;
    Figure& operator=(const Figure& other) {
        if (this != &other) {
            data.clear();
            for (const auto& point_ptr : other.data) {
                if (point_ptr) {
                    data.push_back(std::make_unique<Point<T>>(*point_ptr));
                }
            }
        }
        return *this;
    }
    Figure& operator=(Figure&& other) noexcept = default;
    virtual Point<T> center_count() const {
        if (data.empty()) {
            return Point<T>(0, 0);
        }
        T A = 0;
        size_t array_size = data.size();
        for (size_t i = 0; i < array_size; i++) {
            size_t I = (i + 1) % array_size;
            A += data[i]->get_x() * data[I]->get_y() - data[I]->get_x() * data[i]->get_y();
        }
        A /= 2;
        if (A == 0) {
            T sum_x = 0, sum_y = 0;
            for (const auto& point : data) {
                sum_x += point->get_x();
                sum_y += point->get_y();
            }
            return Point<T>(sum_x / static_cast<T>(array_size), 
                           sum_y / static_cast<T>(array_size));
        }
        T x = 0;
        for (size_t i = 0; i < array_size; i++) {
            size_t I = (i + 1) % array_size;
            x += (data[i]->get_x() + data[I]->get_x()) * 
                 (data[i]->get_x() * data[I]->get_y() - data[I]->get_x() * data[i]->get_y());
        }
        x /= (6 * A);
        T y = 0;
        for (size_t i = 0; i < array_size; i++) {
            size_t I = (i + 1) % array_size;
            y += (data[i]->get_y() + data[I]->get_y()) * 
                 (data[i]->get_x() * data[I]->get_y() - data[I]->get_x() * data[i]->get_y());
        }
        y /= (6 * A);
        return Point<T>(x, y);
    }
    
    bool operator==(const Figure& other) const {
        if (data.size() != other.data.size()) {
            return false;
        }
        for (size_t i = 0; i < data.size(); ++i) {
            if (!data[i] || !other.data[i] || !(*data[i] == *other.data[i])) {
                return false;
            }
        }
        return true;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Figure& fig) {
        for (const auto& point_ptr : fig.data) {
            if (point_ptr) {
                os << "(" << point_ptr->get_x() << ", " << point_ptr->get_y() << ") ";
            }
        }
        return os;
    }
    
    friend std::istream& operator>>(std::istream& is, Figure& fig) {
        fig.data.clear();
        T x, y;
        while (is >> x >> y) {
            fig.data.push_back(std::make_unique<Point<T>>(x, y));
        }
        return is;
    }
    virtual operator double() const = 0; 
    virtual ~Figure() noexcept = default;
    
protected:
    std::vector<std::unique_ptr<Point<T>>> data;
};