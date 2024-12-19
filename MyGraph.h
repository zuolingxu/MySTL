#pragma once
#include "MyVector.h"
#include "MyDeque.h"
#include "MyStack.h"

class MyGraph {
private:
    MyVector<MyVector<int>> adjacency_m;
    size_t size = 0;

public:
    static constexpr int POSITIVE_INF = 0x3fffffff;
    static constexpr int NEGATIVE_INF = -0x3fffffff;

    MyGraph() = default;
    explicit MyGraph(const size_t num) : adjacency_m(MyVector<MyVector<int>>(num, MyVector<int>(num, POSITIVE_INF))), size(num) {}
    ~MyGraph() = default;
    MyGraph(const MyGraph& rhs) = default;
    MyGraph& operator=(const MyGraph& rhs) = default;
    MyGraph(MyGraph&& rhs) = default;
    MyGraph& operator=(MyGraph&& rhs) = default;

    void addVertex(const size_t num = 1) {
        for (int i = 0; i < num; i++) {
            adjacency_m.push_back(MyVector<int>(num, POSITIVE_INF));
        }
        for (auto& v : adjacency_m) {
            for (int j = 0; j < num; j++) {
                v.push_back(POSITIVE_INF);
            }
        }
        size += num;
    }

    void addEdge(const int vertex_from, const int vertex_to, const int weight = 1 , bool directed = false) const {
        if (vertex_from >= 0 && vertex_from < size && vertex_to >= 0 && vertex_to < size) {
            if (adjacency_m[vertex_from][vertex_to] == POSITIVE_INF) {
                if (directed) {
                    adjacency_m[vertex_from][vertex_to] = weight;
                }
                else {
                    if (adjacency_m[vertex_to][vertex_from] == POSITIVE_INF) {
                        adjacency_m[vertex_from][vertex_to] = weight;
                        adjacency_m[vertex_to][vertex_from] = weight;
                    }
                }
            }
        }
    }

    int operator()(const int vertex_from, const int vertex_to) const {
        if (vertex_from >= 0 && vertex_from < size && vertex_to >= 0 && vertex_to < size) {
            return adjacency_m[vertex_from][vertex_to];
        }
        return POSITIVE_INF;
    }

    [[nodiscard]] size_t getVertexNum() const { return size; }

    [[nodiscard]] MyVector<int> BFS(const int start_vertex) const {
        if (size == 0) return {};
        if (start_vertex >= 0 && start_vertex < size) {
            if (size == 1) return MyVector<int>(1, start_vertex);

            MyVector<int> bfs;
            bfs.push_back(start_vertex);
            MyVector<bool> visited(size, false);
            visited[start_vertex] = true;
            MyDeque<int> deque;
            deque.push_back(start_vertex);

            while(!deque.empty()) {
                const int current_v = deque.front();
                deque.pop_front();

                for (int i = 0; i < size; i++) {
                    if (adjacency_m[current_v][i] < POSITIVE_INF && current_v != i && visited[i] == false) {
                        deque.push_back(i);
                        visited[i] = true;
                        bfs.push_back(i);
                    }
                }
            }
            return bfs;
        }
        return {};
    }

    [[nodiscard]] MyVector<int> DFS(const int start_vertex) const {
        if (size == 0) return {};
        if (start_vertex >= 0 && start_vertex < size) {
            if (size == 1) return MyVector<int>(1, start_vertex);

            MyVector<int> dfs;
            dfs.push_back(start_vertex);
            MyVector<bool> visited(size, false);
            visited[start_vertex] = true;
            MyStack<int> stack;
            stack.push(start_vertex);

            while(!stack.empty()) {
                const int current_v = stack.top();
                stack.pop();

                for (int i = 0; i < size; i++) {
                    if (adjacency_m[current_v][i] < POSITIVE_INF && current_v != i && visited[i] == false) {
                        stack.push(i);
                        visited[i] = true;
                        dfs.push_back(i);
                    }
                }
            }
            return dfs;
        }
        return {};
    }

    [[nodiscard]] MyVector<int> Topological_Sort() const {
        if (size == 0) return {};
        if (size == 1) return MyVector<int>(1, POSITIVE_INF);
        MyVector<int> result;
        MyVector<bool> visited(size, false);
        MyGraph temp = *this;
        for (int k = 0; k < size; k++) {
            bool sortable = false;
            for (int i = 0; i < size; i++) {
                if (visited[i] == false) {
                    bool unreachable = true;
                    for (int j = 0; j < size; j++) {
                        if (temp.adjacency_m[j][i] != POSITIVE_INF) {
                            unreachable = false;
                            break;
                        }
                    }
                    if (unreachable) {
                        for (int j = 0; j < size; j++) {
                            temp.adjacency_m[i][j] = POSITIVE_INF;
                        }
                        result.push_back(i);
                        visited[i] = true;
                        sortable = true;
                        break;
                    }
                }
            }
            if (!sortable) {
                return {};
            }
        }
        return result;
    }



    [[nodiscard]] MyVector<int> prim(const int start_vertex) const {
        if (size == 0) return {};
        if (start_vertex >= 0 && start_vertex < size) {
            if (size == 1) return MyVector<int>(1, start_vertex);
            MyVector<int> nearest(size, -1);
            MyVector<bool> visited(size, false);
            visited[start_vertex] = true;
            MyVector<int> distance(size, POSITIVE_INF);
            distance[start_vertex] = 0;
            int current_v = start_vertex;

            while(true) {
                int min_v = -1;
                for (int i = 0; i < size; i++) {
                    if (visited[i] == false) {
                        if (adjacency_m[current_v][i] < distance[i]) {
                            distance[i] = adjacency_m[current_v][i];
                            nearest[i] = current_v;
                        }
                        if (min_v == -1 || distance[i] < distance[min_v]) min_v = i;
                    }
                }
                if (min_v == -1) break;
                visited[min_v] = true;
                current_v = min_v;
            }
            return nearest;
        }
        return {};
    }

    [[nodiscard]] MyVector<int> Dijkstra(const int start_vertex) const {
        if (size == 0) return {};
        if (start_vertex >= 0 && start_vertex < size) {
            if (size == 1) return MyVector<int>(1, start_vertex);
            MyVector<int> nearest_prv(size, -1);
            MyVector<bool> visited(size, false);
            visited[start_vertex] = true;
            MyVector<int> distance(size, POSITIVE_INF);
            distance[start_vertex] = 0;
            int current_v = start_vertex;

            while(true) {
                int min_v = -1;
                for (int i = 0; i < size; i++) {
                    if (visited[i] == false) {
                        if (min_v == -1) min_v = i;
                        if (distance[current_v] + adjacency_m[current_v][i] < distance[i]) {
                            distance[i] = distance[current_v] + adjacency_m[current_v][i];
                            nearest_prv[i] = current_v;
                        }
                        if (distance[i] < distance[min_v]) min_v = i;
                    }
                }
                if (min_v == -1) break;
                visited[min_v] = true;
                current_v = min_v;
            }
            return nearest_prv;
        }
        return {};
    }

    // TODO: removeEdge
    // TODO: removeVertex
    // TODO: string - vertex map
};
