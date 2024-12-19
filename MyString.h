#pragma once
#include <cstring>
#include <iostream>

class MyString {
private:
    static constexpr int START_SIZE = 16;
    static constexpr int MULTIPLE = 2;
    size_t length;
    size_t capacity;
    char* ptr;

    // Helper function to allocate new memory and copy
    void allocate_and_copy(const char* source, size_t new_length) {
        capacity = START_SIZE;
        while (capacity <= new_length) { capacity *= MULTIPLE; }
        ptr = new char[capacity + 1];  // 多分配一个字节用于空字符
        memcpy(ptr, source, new_length);
        ptr[new_length] = '\0';  // 添加空字符结尾
        length = new_length;
    }

public:
    class StringIterator{
        private:
        char* ptr;
        public:
        StringIterator(char* source): ptr(source){}
        StringIterator(): ptr(nullptr){}
        StringIterator(const StringIterator& source) = default;
        ~StringIterator() = default;
        StringIterator& operator=(const StringIterator& source) {
            if (this != &source) { ptr = source.ptr; }
            return *this;
        }
        char& operator*() const { return *ptr; }
        StringIterator& operator++() { ++ptr; return *this; }
        StringIterator& operator--() { --ptr; return *this; }

        StringIterator operator+(const size_t offset) const { return ptr + offset; }
        StringIterator operator-(const size_t offset) const { return ptr - offset; }
        StringIterator& operator+=(const size_t offset) { ptr += offset; return *this; }
        StringIterator& operator-=(const size_t offset) { ptr -= offset; return *this; }

        bool operator==(const StringIterator& source) const { return ptr == source.ptr; }
        bool operator!=(const StringIterator& source) const { return ptr != source.ptr; }
        bool operator<(const StringIterator& source) const { return ptr < source.ptr; }
        bool operator>(const StringIterator& source) const { return ptr > source.ptr; }
        bool operator<=(const StringIterator& source) const { return ptr <= source.ptr; }
        bool operator>=(const StringIterator& source) const { return ptr >= source.ptr; }
    };
    MyString() : length(0), capacity(START_SIZE), ptr(new char[START_SIZE + 1]) { ptr[0] = '\0'; }
    MyString(const MyString& other) { allocate_and_copy(other.ptr, other.length); }
    MyString(const char* other) {
        if (other != nullptr) { allocate_and_copy(other, strlen(other)); }
        else {
            length = 0;
            capacity = START_SIZE;
            ptr = new char[capacity + 1];
            ptr[0] = '\0';
        }
    }
    MyString(const char other) {
        length = 1;
        capacity = START_SIZE;
        ptr = new char[capacity + 1];
        *ptr = other;
        ptr[1] = '\0';  // 添加空字符
    }
    ~MyString() { delete[] ptr; }

    size_t size() const { return length; }
    size_t str_capacity() const { return capacity; }
    bool is_empty() const { return length == 0; }

    MyString& append(const char ch) {
        if (length >= capacity) { reserve(capacity * MULTIPLE); }
        ptr[length] = ch;
        length++;
        ptr[length] = '\0';
        return *this;
    }
    void clear() {
        length = 0;
        ptr[0] = '\0';
    }
    void reserve(const size_t new_capacity) {
        if (new_capacity > capacity) {
            char* temp_ptr = new char[new_capacity + 1];
            memcpy(temp_ptr, ptr, length);
            temp_ptr[length] = '\0';
            delete[] ptr;
            ptr = temp_ptr;
            capacity = new_capacity;
        }
    }

    MyString& operator=(MyString other) {
        std::swap(ptr, other.ptr);  // 使用 swap 技术避免重复内存分配
        std::swap(length, other.length);
        std::swap(capacity, other.capacity);
        return *this;
    }
    MyString& operator=(const char* other) { return *this = MyString(other); }
    MyString& operator=(const char other) { return *this = MyString(other); }

    // Concatenation operators
    MyString operator+(const MyString& other) const {
        MyString temp;
        temp.length = length + other.length;
        temp.capacity = START_SIZE;
        while (temp.capacity <= temp.length) { temp.capacity *= MULTIPLE; }
        temp.ptr = new char[temp.capacity + 1];
        memcpy(temp.ptr, ptr, length);
        memcpy(temp.ptr + length, other.ptr, other.length);
        temp.ptr[temp.length] = '\0';  // 添加空字符
        return temp;
    }
    MyString operator+(const char* other) const { return *this + MyString(other); }
    MyString operator+(const char other) const { return *this + MyString(other); }

    MyString& operator+=(const MyString& other) {
        if (length + other.length > capacity) {
            reserve((length + other.length) * MULTIPLE);
        }
        memcpy(ptr + length, other.ptr, other.length);
        length += other.length;
        ptr[length] = '\0';  // 确保字符串以空字符结尾
        return *this;
    }
    MyString& operator+=(const char* other) { return *this += MyString(other); }
    MyString& operator+=(const char other) { append(other); return *this; }

    bool operator==(const MyString& other) const {
        return length == other.length && memcmp(ptr, other.ptr, length) == 0;
    }
    bool operator!=(const MyString& other) const { return !(*this == other); }
    bool operator<(const MyString& other) const {
        const size_t min_len = length < other.length ? length : other.length;
        for (size_t i = 0; i < min_len; i++) {
            if (ptr[i] != other.ptr[i]) { return ptr[i] < other.ptr[i]; }
        }
        return length < other.length;
    }
    bool operator>(const MyString& other) const {
        const size_t min_len = length < other.length ? length : other.length;
        for (size_t i = 0; i < min_len; i++) {
            if (ptr[i] != other.ptr[i]) { return ptr[i] > other.ptr[i]; }
        }
        return length > other.length;
    }
    bool operator<=(const MyString& other) const { return !(*this > other); }
    bool operator>=(const MyString& other) const { return !(*this < other); }

    StringIterator begin() const { return ptr; }
    StringIterator end() const { return ptr + length; }

    // Subscript operator
    char& operator[](const size_t index) const { return ptr[index]; }

    // Friend functions for I/O
    friend std::ostream& operator<<(std::ostream& out, const MyString& str) {
        out << str.ptr;  // 输出时使用 C 风格字符串
        return out;
    }
    friend std::istream& operator>>(std::istream& in, MyString& str) {
        str.clear();
        char ch;
        while (in.get(ch) && std::isspace(ch)) {}  // 跳过前导空白
        if (!in) return in;
        str.append(ch);
        while (in.get(ch) && !std::isspace(ch)) {
            str.append(ch);
        }
        return in;
    }
    friend std::istream& getline(std::istream& in, MyString& str, char delim = '\n') {
        str.clear();
        char ch;
        while (in.get(ch) && std::isspace(ch)) {}  // 跳过前导空白
        if (!in) return in;
        str.append(ch);
        while (in.get(ch) && ch != delim) { str.append(ch); }
        return in;
    }

};

inline MyString operator+(const char* other, const MyString& str) { return MyString(other) + str; }
inline MyString operator+(const char other, const MyString& str) { return MyString(other) + str; }
