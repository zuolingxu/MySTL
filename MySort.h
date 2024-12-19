#pragma once
#include "MyVector.h"
#include "MyHeap.h"
#include "MyStack.h"

enum SortType {
    bubble = 1,
    choose = 2,
    insert = 3,
    shell = 4,
    quick = 5,
    heap = 6,
    merge = 7,
    radix = 8,
};

class MySort {
private:
    int type;
    static constexpr int SMALL_QUICK = 6;
    static constexpr int SHELL_GAP[14] = {1, 9, 34, 182, 836, 4025, 19001, 90358,
        428481, 2034035, 9651787, 45806244, 217378076, 1031612713};
    template<typename T>
    static void bubble(MyVector<T>& data) {
        const size_t size = data.size();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (data[j] > data[j + 1]) { data.swap(j, j + 1); }
            }
        }
    }

    template<typename T>
    static void choose(MyVector<T>& data) {
        const size_t size = data.size();
        for (int i = 0; i < size; i++) {
            size_t minIndex = i;
            for (int j = i + 1; j < size; j++) {
                if (data[j] < data[minIndex]) { minIndex = j; }
            }
            data.swap(i, minIndex);
        }
    }

    template<typename T>
    static void insert(MyVector<T>& data) {
        const size_t size = data.size();
        for (int i = 0; i < size - 1; ++i) {
            T key = data[i + 1];
            int j = i;
            for (; j >= 0 && data[j] > key; --j) { data[j + 1] = data[j]; }
            data[j + 1] = key;
        }
    }

    template<typename T>
    static void shell(MyVector<T>& data) {
        const int size = data.size();
        int gap_index = 0;
        for (int i = 0; i < 14; i++) {
            if (size/3 < SHELL_GAP[i]) { gap_index = i; }
        }

        for (; gap_index >= 0; --gap_index) {
            const int gap = SHELL_GAP[gap_index];
            for (int i = 0; i < size - gap; i++) {
                T key = data[i + gap];
                int j = i;
                for (; j >= 0 && data[j] > key; j -= gap) { data[j + gap] = data[j]; }
                data[j + gap] = key;
            }
        }
    }

    template<typename T>
    static void quick(MyVector<T>& data) {
        struct pair {
            int first;
            int last;
        };
        MyStack<pair> border;
        border.push({0, static_cast<int>(data.size() - 1)});
        while (!border.empty()) {
            int left = border.top().first;
            int right = border.top().last;
            if (right - left <= SMALL_QUICK) {
                for (int i = left; i < right; ++i) {
                    T key = data[i + 1];
                    int j = i;
                    for (; j >= left && data[j] > key; --j) { data[j + 1] = data[j];}
                    data[j + 1] = key;
                }
                border.pop();
                continue;
            }

            const T key = data[left];
            while (left < right){
                while (left < right && data[right] >= key) { --right; }
                while (left < right && data[left] <= key) { ++left; }
                data.swap(left, right);
            }
            const int mid = left;
            left = border.top().first;
            right = border.top().last;
            data.swap(left, mid);

            border.pop();
            border.push({mid + 1, right});
            border.push({left, mid - 1});
        }
    }

    template<typename T>
    static void heap(MyVector<T>& data){
        sort_Vector(data);
    }

    template<typename T>
    static void merge(MyVector<T>& data) {
        const int size = static_cast<int>(data.size());
        int gap = 1;
        while(gap < size) {
            int left = 0;
            MyVector<T> temp(size, 0);
            while (left + gap < size) {
                const int mid = left + gap;
                const int right = std::min(mid + gap, size);
                int merge1 = left;
                int merge2 = mid;
                for(int i = left; i < right; ++i ) {
                    if (merge2 >= right || (merge1 < mid && data[merge1] < data[merge2])) {
                        temp[i] = data[merge1];
                        merge1++;
                    }
                    else {
                        temp[i] = data[merge2];
                        merge2++;
                    }
                }
                left = right;
            }
            while (left < size) {
                temp[left] = data[left];
                left++;
            }
            data.move(temp);
            gap *= 2;
        }
    }

    static void radix(MyVector<int>& data) {
        int multi = 1;
        while (true) {
            MyVector<int> bucket[10];
            for (const auto& i : data) {
                bucket[i / multi % 10].push_back(i);
            }
            if (bucket[0].size() == data.size()) {
                data.move(bucket[0]);
                break;
            }
            int index = 0;
            for (const auto& i : bucket) {
                for (const auto& j : i) {
                    data[index++] = j;
                }
            }
            multi *= 10;
        }
    }

    template<typename T>
    static void radix(MyVector<T>& data) {
        throw std::invalid_argument("Invalid type for cardinality sort");
    }

public:
    explicit MySort(const int _type = SortType::quick) : type(_type) {}
    ~MySort() = default;

    template<typename T>
    void operator()(MyVector<T>& data) const{
        switch (type) {
            case SortType::bubble:
                bubble(data);
            return;
            case SortType::choose:
                choose(data);
            return;
            case SortType::insert:
                insert(data);
            return;
            case SortType::shell:
                shell(data);
            return;
            case SortType::quick:
                quick(data);
            return;
            case SortType::heap:
                heap(data);
            return;
            case SortType::merge:
                merge(data);
            return;
            case SortType::radix:
                radix(data);
            return;
            default:
                quick(data);
            return;
        }
    }
};

