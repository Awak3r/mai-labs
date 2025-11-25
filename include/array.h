#pragma once
#include <memory>
#include <stdexcept>
#include <utility>

template<class T>
class Array {
public:
    Array() : capacity_(0), size_(0), data_(nullptr) {}
    explicit Array(size_t capacity) 
        : capacity_(capacity), size_(0), data_(std::make_unique<std::shared_ptr<T>[]>(capacity)) {}
    Array(const Array& other) 
        : capacity_(other.capacity_), size_(other.size_), 
          data_(std::make_unique<std::shared_ptr<T>[]>(other.capacity_)) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    Array(Array&& other) noexcept 
        : capacity_(other.capacity_), size_(other.size_), data_(std::move(other.data_)) {
        other.capacity_ = 0;
        other.size_ = 0;
    }
    Array& operator=(const Array& other) {
        if (this != &other) {
            capacity_ = other.capacity_;
            size_ = other.size_;
            data_ = std::make_unique<std::shared_ptr<T>[]>(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }
    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            capacity_ = other.capacity_;
            size_ = other.size_;
            data_ = std::move(other.data_);
            other.capacity_ = 0;
            other.size_ = 0;
        }
        return *this;
    }
    void push_back(std::shared_ptr<T> element) {
        if (size_ >= capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = std::move(element);
    }
    void remove(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        data_[--size_] = nullptr;
    }
    std::shared_ptr<T>& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    const std::shared_ptr<T>& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }
    
    ~Array() = default;

private:
    void resize(size_t new_capacity) {
        auto new_data = std::make_unique<std::shared_ptr<T>[]>(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        data_ = std::move(new_data);
        capacity_ = new_capacity;
    }
    
    size_t capacity_;
    size_t size_;
    std::unique_ptr<std::shared_ptr<T>[]> data_;
};