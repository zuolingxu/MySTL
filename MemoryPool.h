#pragma once
#define START_SIZE 16
#define MULTIPLE 2

template<class T>
class MemoryBlock {
private:
    MemoryBlock* next;
    int capacity;
    int free_num;
    int free_head;
    T* first;

public:
    explicit MemoryBlock(const int start_capacity)
        : capacity(start_capacity), free_num(start_capacity), next(nullptr), free_head(0) {
        first = new T[capacity];
        // 以链表的形式初始化空闲块指针数组
        for (int i = 0; i < free_num - 1; i++) {
            *reinterpret_cast<int*>(first + i) = i + 1;
        }
        *reinterpret_cast<int*>(first + free_num - 1) = -1;  // 最后一个元素指向 -1，表示链表末尾
    }

    ~MemoryBlock() {
        delete[] first;
        delete next;
    }

    T* allocate() {
        if (free_num == 0) {
            if (next == nullptr) {
                next = new MemoryBlock(MULTIPLE * capacity);  // 扩展内存池
            }
            return next->allocate();
        }
        // 分配空闲块
        int index = free_head;
        free_head = *reinterpret_cast<int*>(first + free_head);
        free_num--;
        return first + index;
    }

    void deallocate(T* ptr) {
        if (ptr >= first && ptr < first + capacity) {
            // 回收该块内存
            *reinterpret_cast<int*>(ptr) = free_head;
            free_head = ptr - first;
            free_num++;
        } else {
            if (next != nullptr) {
                next->deallocate(ptr);  // 递归寻找相应的块
                // 如果下一块完全空闲且为链表的末尾，释放它
                if (next->next == nullptr && next->free_num == next->capacity) {
                    delete next;
                    next = nullptr;
                }
            }
        }
    }
};

template<class T>
class MemoryPool {
private:
    MemoryBlock<T>* head;
    int capacity;

public:
    explicit MemoryPool(int start_capacity = START_SIZE)
        : capacity(start_capacity), head(new MemoryBlock<T>(start_capacity)) {}

    ~MemoryPool() {
        delete head;  // delete 会自动调用链表的析构函数
    }

    T* allocate() {
        return head->allocate();
    }

    void deallocate(T* ptr) {
        head->deallocate(ptr);
    }
};