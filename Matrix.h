#pragma once
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <cmath>

template<class T>
class Matrix {
private:
    std::vector<std::vector<T> > data;
    int rows, cols;
    int output_len = 4;

public:
    Matrix() : data(), rows(0), cols(0) {}
    Matrix(int rows, int cols) : data(rows, std::vector<T>(cols, 0)), rows(rows) , cols(cols) {}
    Matrix(const Matrix &other) : data(other.data), rows(other.rows), cols(other.cols) {}
    Matrix(std::initializer_list<std::initializer_list<T> > list) : rows(list.size()) {
        cols = 0;
        for (auto &row : list) {
            data.emplace_back(row.begin(), row.end());
            if (row.size() > cols) { cols = row.size(); }
        }
        for (auto &row: data) { row.resize(cols, 0); }
    }
    explicit Matrix(std::vector<std::vector<T>>& other) : rows(other.size()) {
        cols = 0;
        for (auto &row : other) {
            data.emplace_back(row.begin(), row.end());
            if (row.size() > cols) { cols = row.size(); }
        }
        for (auto &row: data) { row.resize(cols, 0); }
    }

    ~Matrix() = default;


    Matrix &operator=(const Matrix &other) {
        if (this != &other) {
            rows = other.rows;
            cols = other.cols;
            data = other.data;
        }
        return *this;
    }
    Matrix &operator=(Matrix &&other) noexcept {
        rows = other.rows;
        other.rows = 0;
        cols = other.cols;
        other.cols = 0;
        data = std::move(other.data);
        return *this;
    }


    bool operator==(const Matrix &other) const {
        if (rows != other.rows) { return false; }
        if (cols != other.cols) { return false; }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (data[i][j] != other.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    bool operator!=(const Matrix &other) const { return !(*this == other); }

    Matrix operator+(const Matrix &other) const {
        if (rows != other.rows || cols != other.cols) { throw std::runtime_error("matrix size mismatch"); }
        Matrix temp = *this;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { temp.data[i][j] += other.data[i][j]; }
        }
        return temp;
    }
    Matrix &operator+=(const Matrix &other) {
        if (rows != other.rows || cols != other.cols) { throw std::runtime_error("matrix size mismatch"); }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { data[i][j] += other.data[i][j]; }
        }
        return *this;
    }

    Matrix operator+(const T &other) const {
        Matrix temp = *this;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { temp.data[i][j] += other; }
        }
        return temp;
    }
    Matrix &operator+=(const T &other) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { data[i][j] += other; }
        }
        return *this;
    }

    Matrix operator-(const Matrix &other) const {
        if (rows != other.rows || cols != other.cols) { throw std::runtime_error("matrix size mismatch"); }
        Matrix temp = *this;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { temp.data[i][j] -= other.data[i][j]; }
        }
        return temp;
    }
    Matrix &operator-=(const Matrix &other) {
        if (rows != other.rows || cols != other.cols) { throw std::runtime_error("matrix size mismatch"); }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { data[i][j] -= other.data[i][j]; }
        }
        return *this;
    }

    Matrix operator-(const T &other) const {
        Matrix temp = *this;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { temp.data[i][j] -= other; }
        }
        return temp;
    }
    Matrix &operator-=(const T &other) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { data[i][j] -= other; }
        }
        return *this;
    }

    Matrix operator*(const Matrix &other) const  {
        if (cols != other.rows) { throw std::runtime_error("matrix size mismatch"); }
        Matrix temp(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    temp.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return temp;
    }
    Matrix &operator*=(const Matrix &other) { return *this = *this * other; }

    Matrix operator*(const T &other) const {
        Matrix temp = *this;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { temp.data[i][j] *= other; }
        }
        return temp;
    }
    Matrix &operator*=(const T &other) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { data[i][j] *= other; }
        }
        return *this;
    }

    Matrix operator/(const T &other) const {
        Matrix temp = *this;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { temp.data[i][j] /= other; }
        }
        return temp;
    }
    Matrix &operator/=(const T &other) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { data[i][j] /= other; }
        }
        return *this;
    }

    Matrix operator^(int exp) const {
        if (rows != cols) { throw std::runtime_error("matrix size mismatch"); }
        if (exp < 0) { throw std::runtime_error("negative exponents are not supported"); }
        Matrix result(rows, cols);
        Matrix base = *this;
        for (int i = 0; i < rows; i++) { result.data[i][i] = 1; }
        while (exp > 0) {
            if (exp & 1) { result *= base; }
            base *= base;
            exp >>= 1;
        }
        return result;
    }
    Matrix operator^=(int exp) {
        if (rows != cols) { throw std::runtime_error("matrix size mismatch"); }
        if (exp < 0) { throw std::runtime_error("negative exponents are not supported"); }
        Matrix base = *this;
        data.fill(rows, std::vector<T>(cols, 0));
        for (int i = 0; i < rows; i++) { data[i][i] = 1; }
        while (exp > 0) {
            if (exp & 1) { *this *= base; }
            base *= base;
            exp >>= 1;
        }
        return *this;
    }

    void setRows(const int input_rows) {
        rows = input_rows;
        data.resize(rows, std::vector<T>(cols, 0));
    }
    void setCols(const int input_cols) {
        cols = input_cols;
        for (auto &row: data) { row.resize(cols, 0); }
    }

    int col() const { return cols; }
    int row() const { return rows; };

    T &operator()(const int row, const int col) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) { throw std::out_of_range("index out of range"); }
        return data[row][col];
    }
    T operator()(const int row, const int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) { throw std::out_of_range("index out of range"); }
        return data[row][col];
    }

    Matrix transpose() const {
        Matrix temp(cols, rows);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) { temp.data[j][i] = data[i][j]; }
        }
        return temp;
    }

    void setOutputLen(const int len) { output_len = len; }

    friend std::istream &operator>>(std::istream &in, Matrix &m){
        std::cout << "Enter matrix: ";
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++) { in >> m(i, j); }
        }
        return in;
    }
    friend std::ostream &operator<<(std::ostream &out, const Matrix &m){
        if (m.rows == 0 || m.cols == 0) {
            out << "NULL" << std::endl;
            return out;
        }
        const int row_len = static_cast<int>(std::log10(m.rows) + 1);
        const int col_len = std::max(static_cast<int>(std::log10(m.cols) + 1), m.output_len);
        out << std::setw(row_len) << "M" << ' ';
        for (int i = 0; i < m.cols; i++) {
            out << std::setw(col_len) << i << ' ';
        }
        out << std::endl;
        for (int i = 0; i < m.rows; i++) {
            out << std::setw(row_len) << i << ' ';
            for (int j = 0; j < m.cols; j++) { out << std::setw(m.output_len) << m(i, j) << " "; }
            out << std::endl;
        }
        return out;
    }
};
