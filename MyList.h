#pragma once
#include "MemoryPool.h"

template<class T>
class MyBinaryTree {
private:
    struct Node {
        Node* next;
        Node* prev;
        T data;
    };

    MemoryPool<Node> list_pool;
    Node* head; // 指向链表的第一个节点
    Node* tail; // 指向链表的尾部哨兵节点
    size_t size_;

public:
    class list_iterator {
        private:
            Node* current;
        public:
            list_iterator(Node* ptr) : current(ptr) {}
            list_iterator(const list_iterator& other) : current(other.current) {}

            list_iterator& operator++() {
                if (current->next != nullptr)
                    current = current->next;
                return *this;
            }
            list_iterator& operator--() {
                if (current->prev != nullptr)
                    current = current->prev;
                return *this;
            }

            T& operator*() { return current->data; }  // 直接返回数据
            bool operator==(const list_iterator& other) { return current == other.current; }
            bool operator!=(const list_iterator& other) { return current != other.current; }

            Node* get_node() const { return current; }  // 返回内部节点
    };

    MyBinaryTree() : size_(0) {
        Node* sentinel = list_pool.allocate(); // 分配哨兵节点
        sentinel->next = nullptr;
        sentinel->prev = nullptr;
        head = sentinel;
        tail = sentinel;
    }
    MyBinaryTree(const MyBinaryTree& other) : MyBinaryTree() {
        Node* sentinel = list_pool.allocate();
        sentinel->next = nullptr;
        sentinel->prev = nullptr;
        head = sentinel;
        tail = sentinel;
        for(list_iterator i = other.begin(); i != other.end(); ++i) { push_back(*i); }
    }
    ~MyBinaryTree() { clear(); list_pool.deallocate(tail); } // 清除所有节点并释放哨兵节点

    MyBinaryTree& operator=(const MyBinaryTree& other) {
        if (this != &other) {
            clear();
            for(list_iterator i = other.begin(); i != other.end(); ++i) { push_back(*i); }
        }
        return *this;
    }

    void push_back(const T& new_data) {
        Node* temp = list_pool.allocate();
        temp->data = new_data;
        temp->next = tail;
        temp->prev = tail->prev;

        if (tail->prev) { tail->prev->next = temp; }
        else { head = temp; }

        tail->prev = temp;
        size_++;
    }
    void push_front(const T& new_data) {
        Node* temp = list_pool.allocate();
        temp->data = new_data;
        temp->next = head;
        temp->prev = nullptr;

        if (head != tail) { head->prev = temp; }
        else { tail->prev = temp; }

        head = temp;
        size_++;
    }
    void pop_back() {
        if (size_ == 0) return;

        Node* last_node = tail->prev;
        if (last_node->prev) {last_node->prev->next = tail; }
        else { head = tail; }

        tail->prev = last_node->prev;
        list_pool.deallocate(last_node);
        size_--;
    }
    void pop_front() {
        if (size_ == 0) return;

        Node* first_node = head;
        if (first_node->next != tail) {
            head = first_node->next;
            head->prev = nullptr;
        } else {
            head = tail; // 如果只有一个元素，头指向尾哨兵
            tail->prev = nullptr;
        }

        list_pool.deallocate(first_node);
        size_--;
    }

    list_iterator insert(list_iterator& insert_it, const T& new_data) {
        Node* next_node = insert_it.get_node();
        if (next_node == head) {
            push_front(new_data);
            return list_iterator(head);
        }
        if (next_node == tail) {
            push_back(new_data);
            return list_iterator(tail->prev);
        }

        Node* prev_node = next_node->prev;
        Node* temp = list_pool.allocate();
        temp->data = new_data;
        temp->next = next_node;
        temp->prev = prev_node;

        prev_node->next = temp;
        next_node->prev = temp;

        size_++;
        return list_iterator(temp);
    }
    list_iterator erase(list_iterator& erase_it) {
        Node* node_to_erase = erase_it.get_node();

        if (node_to_erase == head) {
            pop_front();
            return list_iterator(head);
        }
        if (node_to_erase == tail) {
            return list_iterator(tail); // 不能删除哨兵节点
        }

        Node* prev_node = node_to_erase->prev;
        Node* next_node = node_to_erase->next;

        prev_node->next = next_node;
        next_node->prev = prev_node;

        list_pool.deallocate(node_to_erase);
        size_--;
        return list_iterator(next_node);
    }
    list_iterator find(const T& target_data) {
        list_iterator it = begin();
        while (it != end()) {
            if (*it == target_data) { return it; }
            ++it;
        }
        return it;
    }

    size_t list_size() const { return size_; }
    void clear() {
        while (head != tail) {
            Node* temp = head->next;
            list_pool.deallocate(head);
            head = temp;
        }
        size_ = 0;
    }
    bool empty() const { return size_ == 0; }

    list_iterator begin() const { return list_iterator(head); }
    list_iterator end() const { return list_iterator(tail); }

    void sort(bool (*smaller_than)(const T& first,const T& next)) {
        if (size_ < 2) return;  // 如果链表为空或只有一个元素，则无需排序
        for (size_t i = 0; i < size_; ++i) {
            list_iterator current = begin();
            list_iterator next = current;
            ++next;
            for (size_t j = 0; j < size_ - 1; ++j) {
                if (!smaller_than(*current, *next)){  // 如果当前节点大于下一个节点，则交换
                    T temp = current.get_node()->data;
                    current.get_node()->data = next.get_node()->data;
                    next.get_node()->data = temp;
                }
                ++current;
                ++next;
            }
        }
    }
};
