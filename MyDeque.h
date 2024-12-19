#pragma once
#include <stdexcept>


template<class T>
class MyDeque {
public:
    class DequeIterator {
    private:
        T* current;
        T* first;
        size_t unit_size;
        T** node;
        MyDeque* parent;

    public:
        friend class MyDeque;
        DequeIterator() : current(nullptr), first(nullptr), unit_size(0), node(nullptr), parent(nullptr) {}
        DequeIterator(MyDeque* p, T** cur_node, T* cur, T* start)
            : current(cur), first(start), unit_size(p->unit_size), node(cur_node), parent(p) { }

        DequeIterator& operator++() {
            if (current == nullptr) {
                throw std::out_of_range("Iterator cannot be incremented past end");
            }

            ++current;
            if (current == first + unit_size) {
                ++node;
                if (node == parent->data + parent->capacity) {
                    // 到达末尾，设置为 end()
                    current = nullptr;
                    first = nullptr;
                    node = nullptr;
                } else {
                    first = *node;
                    current = first;
                }
            }
            return *this;
        }
        DequeIterator& operator--() {
            if (current == nullptr) {
                // 如果迭代器在 end()，则移动到最后一个元素
                if (parent->size() == 0) {
                    throw std::out_of_range("Cannot decrement iterator in empty deque");
                }
                node = parent->last.node;
                first = parent->last.first;
                current = parent->last.current - 1;
                return *this;
            }

            if (current == first) {
                if (node == parent->data) {
                    throw std::out_of_range("Iterator cannot be decremented past begin");
                }
                --node;
                first = *node;
                current = first + unit_size;
            }
            --current;
            return *this;
        }

        T& operator*() { return *current; }
        T* operator->() { return current; }

        bool operator==(const DequeIterator& other) const { return current == other.current; }
        bool operator!=(const DequeIterator& other) const { return !(*this == other); }

        DequeIterator operator+(const size_t n) const {
            // 计算当前迭代器的位置索引
            const size_t block_index = node - parent->data;
            const size_t offset_in_block = current - first;
            const size_t total_index = block_index * unit_size + offset_in_block + n;

            // 计算新的块索引和块内偏移
            size_t new_block_index = total_index / unit_size;
            size_t new_offset_in_block = total_index % unit_size;

            // 更新指针以指向新位置
            T** new_node = parent->data + new_block_index;
            T* new_first = *new_node;
            T* new_current = new_first + new_offset_in_block;

            return DequeIterator(parent, new_node, new_current, new_first);
        }
        DequeIterator operator-(const size_t n) const { return *this + (-n); }

        DequeIterator& operator+=(size_t distance) { return *this = *this + distance; }
        DequeIterator& operator-=(size_t distance) { return *this = *this - distance; }

        DequeIterator& refresh_iterator(MyDeque* p, T** cur_node, T* cur, T* start) {
            parent = p;
            node = cur_node;
            current = cur;
            first = start;
            unit_size = p->unit_size;
            return *this;
        }
    };

private:
    static constexpr int DATA_ARRAY_SIZE = 16;
    static constexpr int POINTER_ARRAY_SIZE = 16;
    static constexpr int CAPACITY_MULTIPLIER = 2;
    T** data;
    size_t unit_size;
    size_t size_;
    size_t capacity;
    size_t data_capacity;
    DequeIterator first;
    DequeIterator last;

public:
    MyDeque()
        : unit_size(DATA_ARRAY_SIZE), size_(0), capacity(1), data_capacity(POINTER_ARRAY_SIZE + 1) {
        data = new T*[data_capacity]();
        data[0] = new T[unit_size];
        first.refresh_iterator(this, &data[0], data[0], data[0]);
        last = first;
    }
    MyDeque(const MyDeque& other)
        : unit_size(other.unit_size), size_(0), capacity(1),data_capacity(POINTER_ARRAY_SIZE) {
        data = new T*[data_capacity]();
        data[0] = new T[unit_size];
        first.refresh_iterator(this, &data[0], data[0], data[0]);
        last.refresh_iterator(this, &data[0], data[0], data[0]);
        DequeIterator it = other.first;
        while (it != other.last) {
            push_back(*it);
            ++it;
        }
    }
    ~MyDeque() {
        for (size_t i = 0; i < capacity; i++) {
            delete[] data[i];
        }
        delete[] data;
    }

    MyDeque& operator=(const MyDeque& other) {
        if (this != &other) {
            unit_size = other.unit_size;
            clear();
            DequeIterator it = other.first;
            while (it != other.last) {
                push_back(*it);
                ++it;
            }
        }
        return *this;
    }
    T& operator[](size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        DequeIterator temp = first + index;
        return *temp;
    }

    DequeIterator begin() { return first; }
    DequeIterator end() { return last; }

    [[nodiscard]] bool empty() const { return size_ == 0; }
    [[nodiscard]] size_t size() const { return size_; }
    void clear() {
        for (size_t i = 0; i < capacity; ++i) {
            delete[] data[i];
        }
        capacity = 1;
        data[0] = new T[unit_size];
        size_ = 0;
        first.refresh_iterator(this, &data[0], data[0], data[0]);
        last.refresh_iterator(this, &data[0], data[0], data[0]);
    }

    void push_back(const T& value) {
        if (last.current == last.first + unit_size - 1) {
            if (capacity == data_capacity) { enlarge_data_capacity(); }
            data[capacity] = new T[unit_size];
            ++capacity;
        }
        *last = value;
        ++last;
        ++size_;
    }
    void push_front(const T& value) {
        if (first.current == first.first) {
            if (capacity == data_capacity) { enlarge_data_capacity(); }
            for (size_t i = capacity; i > 0; --i) { data[i] = data[i - 1]; }
            data[0] = new T[unit_size];
            ++capacity;
            first.refresh_iterator(this, &data[0], data[0] + unit_size, data[0]);
            ++last.node;
        }
        --first;
        *first = value;
        ++size_;
    }

    void pop_back() {
        if (empty()) { throw std::out_of_range("Deque is empty"); }
        --last;
        --size_;
    }
    void pop_front() {
        if (empty()) { throw std::out_of_range("Deque is empty"); }
        ++first;
        --size_;
    }

    T& front() { return *first; }
    T& back() { DequeIterator temp = last; --temp; return *temp; }
private:
    void enlarge_data_capacity() {
        const size_t new_capacity = data_capacity * CAPACITY_MULTIPLIER;
        T** new_data = new T*[new_capacity]();
        for (size_t i = 0; i < capacity; ++i) {
            new_data[i] = data[i];
            if(&data[i] == first.node) { first.node = &new_data[i]; }
            if (&data[i] == last.node) { last.node = &new_data[i]; }
        }
        delete[] data;
        data = new_data;
        data_capacity = new_capacity;
    }
};
