#pragma once
#include "MyDeque.h"
#include <stdexcept>

template<class T>
class MyStack {
private:
    MyDeque<T> container;  // 使用 MyDeque 作为底层容器

public:
    MyStack() = default;
    MyStack(const MyStack& other) : container(other.container) {}
    ~MyStack()  = default;
    MyStack& operator=(const MyStack& other) {
        if (this != &other) { container = other.container; }
        return *this;
    }

    void push(const T& value) { container.push_back(value);}
    void pop() {
        if (container.empty()) { throw std::out_of_range("Stack is empty"); }
        container.pop_back();
    }

    T& top() {
        if (container.empty()) { throw std::out_of_range("Stack is empty"); }
        return container.back();
    }
    const T& top() const {
        if (container.empty()) { throw std::out_of_range("Stack is empty"); }
        return container.back();
    }

    bool empty() const { return container.empty(); }
    size_t size() const { return container.size(); }
    void clear() { container.clear(); }
};
