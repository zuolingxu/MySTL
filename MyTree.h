#pragma once
#include "MyStack.h"

template<class T>
class MyTree {
public:
    struct Node {
        Node* lchild = nullptr;
        Node* rchild = nullptr;
        Node* parent = nullptr;
        T data;
    };

private:
    Node* root;

public:
    class TreeIterator {
        private:
        Node* current;
        public:
        TreeIterator(): current(nullptr) {}
        explicit TreeIterator(Node* node){ current = node; }
        TreeIterator(const TreeIterator& iter) = default;
        TreeIterator& operator=(const TreeIterator& iter) = default;
        ~TreeIterator() = default;
        bool operator==(const TreeIterator & other) const {return current == other.current;}

        TreeIterator& to_left() {
            if (current->lchild != nullptr) { current = current->lchild; }
             return *this;
        }
        TreeIterator& to_right() {
            if (current->rchild != nullptr) { current = current->rchild; }
            return *this;
        }
        TreeIterator& back() {
            if (current->parent != nullptr) { current = current->parent; }
            return *this;
        }
        T& operator*() const { return current->data; }
        Node* operator->() const { return current; }
        Node* get_node() const { return current; }
    };
    class PreIterator {
        private:
        MyStack<TreeIterator> stack;
        public:
        explicit PreIterator(const TreeIterator& it) { stack.push(it); }
        ~PreIterator() = default;

        TreeIterator operator++() {
            if (!stack.empty()) {
                TreeIterator temp = stack.top();
                stack.pop();
                if (temp->rchild != nullptr) { stack.push(TreeIterator(temp->rchild)); }
                if (temp->lchild != nullptr) { stack.push(TreeIterator(temp->lchild)); }
            }
            if(!stack.empty()) {  return stack.top(); }
            else { return TreeIterator(); }
        }

        bool is_end() { return stack.empty(); }
        T& operator*() {
            if (stack.empty()) { throw std::out_of_range("The traverse has been done"); }
            return *(stack.top());
        }
        TreeIterator top() { return stack.top(); }
    };
    class InIterator {
        private:
        MyStack<TreeIterator> stack;
        public:
        explicit InIterator(const TreeIterator& it) {
            stack.push(it);
            TreeIterator temp = it;
            while (temp->lchild != nullptr) {
                temp.to_left();
                stack.push(temp);
            }
        }
        ~InIterator() = default;

        TreeIterator operator++() {
            if (!stack.empty()) {
                TreeIterator temp = stack.top();
                stack.pop();
                if (temp->rchild != nullptr) {
                    stack.push(temp.to_right());
                    while (temp->lchild != nullptr) { stack.push(temp.to_left()); }
                }
            }
            if(!stack.empty()) {  return stack.top(); }
            else { return TreeIterator(); }
        }

        T& operator*() {
            if (stack.empty()) { throw std::out_of_range("The traverse has been done"); }
            return *(stack.top());
        }
        bool is_end() { return stack.empty(); }
        TreeIterator top() { return stack.top(); }
    };
    class PostIterator {
        private:
        class post {
        public:
            TreeIterator it;
            bool count = false;
            post(): it(nullptr) {};
            ~post() = default;
            explicit post(TreeIterator iter) : it(iter) {}
        };
        MyStack<post> stack;
        public:
        explicit PostIterator(const TreeIterator& it) {
            stack.push(post(it));
            TreeIterator temp = it;
            while (temp->lchild != nullptr || temp->rchild != nullptr) {
                while(temp->lchild != nullptr) { stack.push(post(temp.to_left())); }
                stack.top().count = true;
                if(temp->rchild != nullptr) { stack.push(post(temp.to_right())); }
            }
            stack.top().count = true;
        }
        ~PostIterator() = default;

        TreeIterator operator++() {
            if (stack.top().count) { stack.pop(); }
            if (!stack.empty()) {
                TreeIterator temp = stack.top().it;
                while (!stack.top().count) {
                    stack.top().count = true;
                    if (temp->rchild != nullptr) {
                        stack.push(post(temp.to_right()));
                        while (temp->lchild != nullptr) { stack.push(post(temp.to_left())); }
                    }
                }
            }
            if(!stack.empty()) { return stack.top().it; }
            else { return TreeIterator(); }
        }

        T& operator*() {
            if (stack.empty()) { throw std::out_of_range("The traverse has been done"); }
            return *(stack.top().it);
        }
        bool is_end() { return stack.empty(); }
        TreeIterator top() { return stack.top().it; }

    };

    MyTree() { root = new Node; }
    explicit MyTree(const T& data) {
        root = new Node;
        root->data = data;
    }
    ~MyTree() {
        delete_tree(begin());
        root = nullptr;
    }

    TreeIterator add_lchild(TreeIterator it,const T& data) {
        it->lchild = new Node;
        TreeIterator temp(it);
        it.to_left();
        it->parent = temp.get_node();
        *it = data;
        return it;
    }
    TreeIterator add_rchild(TreeIterator it,const T& data) {
        it->rchild = new Node;
        TreeIterator temp(it);
        it.to_right();
        it->parent = temp.get_node();
        *it = data;
        return it;
    }
    void delete_tree(TreeIterator it){
        TreeIterator temp(it);
    	auto fun = [](TreeIterator iter)->bool{
	        delete iter.get_node();
	        return true;
	    };
        it.back();
        if (it->lchild == temp.get_node()) { it->lchild = nullptr; }
        else {it->rchild = nullptr;}
    	postorder(temp , fun);
    }

    TreeIterator begin() { return TreeIterator(root); }
    TreeIterator find(const T& data) {
        PreIterator iter(this->begin());
        while (!iter.is_end()) {
            if (*iter == data) return iter.top();
            ++iter;
        }
        return TreeIterator();
    }

    void preorder(const TreeIterator it, bool (*fun)(TreeIterator)) {
        MyStack<TreeIterator> stack;
        stack.push(it);
        while (!stack.empty()) {
            TreeIterator temp = stack.top();
            stack.pop();
            if (temp->rchild != nullptr) { stack.push(TreeIterator(temp->rchild)); }
            if (temp->lchild != nullptr) { stack.push(TreeIterator(temp->lchild)); }
            if(!fun(temp)) break;
        }
    }
    void inorder(const TreeIterator it, bool (*fun)(TreeIterator)) {
        MyStack<TreeIterator> stack;
        stack.push(it);
        TreeIterator temp = it;
        while (!stack.empty()) {
            while (temp->lchild != nullptr) { stack.push(temp.to_left()); }
            temp = stack.top();
            TreeIterator temp2 = temp;
            stack.pop();
            if (temp->rchild != nullptr) { stack.push(TreeIterator(temp.to_right())); }
            if (!fun(temp2)) break;
        }
    }
    void postorder(const TreeIterator it, bool (*fun)(TreeIterator)) {
        class post {
        public:
            TreeIterator it;
            bool count = false;
            post(): it(nullptr) {};
            ~post() = default;
            explicit post(TreeIterator iter) : it(iter) {}
        };
        MyStack<post> stack;
        stack.push(post(it));
        TreeIterator temp = it;
        while (temp->lchild != nullptr) { stack.push(post(temp.to_left())); }
        while (!stack.empty()) {
            temp = stack.top().it;
            TreeIterator temp2 = temp;
            if (!stack.top().count) {
                stack.top().count = true;
                if (temp->rchild != nullptr) {
                    stack.push(post(temp.to_right()));
                    while (temp->lchild != nullptr) { stack.push(post(temp.to_left())); }
                }
            }
            else {
                stack.pop();
                if (!fun(temp2)) break;
            }
        }
    }
};
