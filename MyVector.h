#pragma once
#include <cstddef>


template<class T>
class MyVector {
private:
    static constexpr int START_SIZE = 16;
    size_t size_{};
    size_t capacity_{START_SIZE};
    T* ptr;

public:
    class VectorIterator{
        private:
        T* ptr;
        public:
        VectorIterator(): ptr(nullptr){}
        explicit VectorIterator(T* ptr_) : ptr(ptr_){}
        VectorIterator(const VectorIterator& source) = default;
        ~VectorIterator() = default;
        VectorIterator& operator=(const VectorIterator& source) {
            if (this != &source) { ptr = source.ptr; }
            return *this;
        }
        T& operator*() const { return *ptr; }
        VectorIterator& operator++() { ++ptr; return *this; }
        VectorIterator& operator--() { --ptr; return *this; }

        VectorIterator operator+(const size_t offset) const { return VectorIterator(ptr + offset); }
        VectorIterator operator-(const size_t offset) const { return VectorIterator(ptr - offset); }
        VectorIterator& operator+=(const size_t offset) { ptr += offset; return *this; }
        VectorIterator& operator-=(const size_t offset) { ptr -= offset; return *this; }

        bool operator==(const VectorIterator& source) const { return ptr == source.ptr; }
        bool operator!=(const VectorIterator& source) const { return ptr != source.ptr; }
        bool operator<(const VectorIterator& source) const { return ptr < source.ptr; }
        bool operator>(const VectorIterator& source) const { return ptr > source.ptr; }
        bool operator<=(const VectorIterator& source) const { return ptr <= source.ptr; }
        bool operator>=(const VectorIterator& source) const { return ptr >= source.ptr; }
    };
    MyVector() : ptr(new T[START_SIZE]) {}
    MyVector(const size_t num, const T& value) : size_(num) {
        capacity_ = START_SIZE;
        while (capacity_ < num) { capacity_ *= 2; }
        ptr = new T[capacity_];
        for (size_t i = 0; i < size_; i++) { ptr[i] = value; }
    }
    MyVector(const MyVector& other) {
        capacity_ = other.capacity_;
        size_ = other.size_;
        ptr = new T[capacity_];
        for (size_t i = 0; i < size_; i++) { ptr[i] = other.ptr[i]; }
    }
    MyVector(MyVector&& other) noexcept {
        capacity_ = other.capacity_;
        size_ = other.size_;
        ptr = other.ptr;
        other.size_ = 0;
        other.ptr = nullptr;
        other.capacity_ = 0;
    }

    MyVector& move(MyVector& other) noexcept {
        capacity_ = other.capacity_;
        size_ = other.size_;
        ptr = other.ptr;
        other.size_ = 0;
        other.ptr = nullptr;
        other.capacity_ = 0;
        return *this;
    }

    ~MyVector() { delete[] ptr; }

    [[nodiscard]] size_t size() const { return size_; }
    [[nodiscard]] size_t capacity() const { return capacity_; }
    [[nodiscard]] bool is_empty() const { return size_ == 0; }

    MyVector& push_back(const T& data) {
        if (size_ >= capacity_) { reserve(capacity_); }
        ptr[size_] = data;
        size_++;
        return *this;
    }
    void pop_back() {
        if(size_ > 0) size_--;
        ptr[size_].~T();
    }
    MyVector& push_front(const T& data) {
        if (size_ >= capacity_) { reserve(capacity_); }
        for(int i = size_; i > 0; i--) { ptr[i] = ptr[i - 1]; }
        ptr[0] = data;
        size++;
        return *this;
    }
    void pop_front() {
        if(size_ > 0) size_--;
        for(int i = 0; i < size_; i++) { ptr[i] = ptr[i + 1]; }
    }
    void swap(int index1,int index2) {
        if(index1 != index2) {
            T temp = ptr[index2];
            ptr[index2] = ptr[index1];
            ptr[index1] = temp;
        }
    }
    void insert(int index, const T& data) {
        if (size_ >= capacity_) { reserve(capacity_); }
        for(int i = size_; i > index; i--) { ptr[i] = ptr[i - 1]; }
        ptr[index] = data;
        size++;
    }

    T& front() { return *ptr; }
    const T& front() const { return *ptr; }
    T& back() { return *(ptr + size_ - 1); }
    const T& back() const { return *(ptr + size_ - 1); }

    void clear() {
        for (size_t i = 0; i < size_; i++) { ptr[i].~T(); }
        size_ = 0;
    }
    void reserve(const size_t add_capacity) {
        capacity_ += add_capacity > capacity_ ? add_capacity : capacity_;
        T* new_ptr = new T[capacity_];
        for (size_t i = 0; i < size_; i++) { new_ptr[i] = ptr[i]; }
        delete[] ptr;
        ptr = new_ptr;
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] ptr;
            capacity_ = other.capacity_;
            size_ = other.size_;
            ptr = new T[capacity_];
            for (size_t i = 0; i < size_; i++) { ptr[i] = other.ptr[i]; }
        }
        return *this;
    }

    VectorIterator begin() const { return VectorIterator(ptr); }
    VectorIterator end() const { return VectorIterator(ptr + size_); }

    // Subscript operator
    T& operator[](const size_t index) const { return ptr[index]; }
};
