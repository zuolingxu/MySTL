#pragma once
#include "MyVector.h"

template <typename T>
class MyMaxHeap {
    private:
    MyVector<T> heap;
    size_t size_{};

    void heapify() {
        const int start = parent(size_ - 1);
        for (int i = start; i >= 0; --i) { heapifyDown(i); }
    }
    void heapifyUp(int index) {
        while(index > 0) {
            int _parent = parent(index);
            if (heap[_parent] < heap[index]) {
                heap.swap(_parent, index);
                index = _parent;
            }
            else return;
        }
    }
    void heapifyDown(int index) {
        while(true) {
            int l_child = lChild(index);
            int r_child = rChild(index);
            if (l_child >= size_) { return; }
            int largest = l_child;
            if (r_child < size_) { largest = heap[r_child] > heap[l_child] ? r_child : l_child; }
            if (heap[index] < heap[largest]) {
                heap.swap(index, largest);
                index = largest;
            }
            else return;
        }
    }
    static int parent(const int index) { return (index - 1) / 2; }  // 父节点索引
    static int lChild(const int index) { return 2 * index + 1; }  // 左子节点索引
    static int rChild(const int index) { return 2 * index + 2; }  // 右子节点索引
    public:
    MyMaxHeap() = default;
    explicit MyMaxHeap(const MyVector<T> other) {
        heap = other;
        size_ = other.size();
        heapify();
    }
    ~MyMaxHeap() = default;
    MyMaxHeap(const MyMaxHeap& rhs) = default;
    MyMaxHeap& operator=(const MyMaxHeap& rhs) = default;
    MyMaxHeap(MyMaxHeap&& rhs) = default;

    void push(const T& value) {
        heap.push_back(value);
        size_++;
        heapifyUp(size_ - 1);
    }
    void pop() {
        heap[0] = heap[size_ - 1];
        size_--;
        heapifyDown(0);
        heap.pop_back();
    }
    T& top() { return heap.front(); }
    const T& top() const { return heap.front();}
    [[nodiscard]] bool isEmpty() const { return heap.is_empty(); }
    [[nodiscard]] int size() const { return size_; }
    void clear() { heap.clear(); size_ = 0; }

    MyVector<T> sort() {
        const int temp = size_;
        for (int i = temp - 1; i > 0; --i) {
            heap.swap(0, i);
            --size_;
            heapifyDown(0);
        }
        size_ = temp;
        return heap;
    }
    template <typename U>
    friend MyVector<U> sort_Vector(MyVector<U>& data);

};

template <typename T>
MyVector<T> sort_Vector(MyVector<T>& data) {
    MyMaxHeap<T> temp;
    temp.size_ = data.size();
    temp.heap.move(data);
    temp.heapify();
    temp.sort();
    data.move(temp.heap);
    return data;
}

template <class T>
class MyMinHeap {
    private:
    class inverse_T {
    private:
        T data;

    public:
        inverse_T() = default;
        inverse_T(const T& other) : data(other) {}
        ~inverse_T() = default;
        bool operator<(const inverse_T& other) const { return data > other.data; }
        bool operator>(const inverse_T& other) const { return data < other.data; }
        T& operator*(){ return data; }
    };
    MyMaxHeap<inverse_T> heap;

    public:
    void push(const T& value) { heap.push(value); }
    void pop() { heap.pop(); }
    T& top() { return *(heap.top()); }
    const T& top() const { return *(heap.top()); }
    [[nodiscard]] bool isEmpty() const { return heap.isEmpty(); }
    [[nodiscard]] int size() const { return heap.size(); }
    void clear() { heap.clear(); }
    MyVector<T> sort() { return heap.sort(); }
};



