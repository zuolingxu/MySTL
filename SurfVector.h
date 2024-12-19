#pragma once
#include <iostream>

template<class T>
class SurfVector {
private:
    T x_;
    T y_;
public:
    SurfVector() = default;
    constexpr SurfVector(const T& _x, const T& _y) : x_(_x), y_(_y) {}
    ~SurfVector() = default;
    SurfVector operator+(const SurfVector &other) const { return SurfVector(x_ + other.x_, y_ + other.y_); }
    SurfVector operator-(const SurfVector &other) const { return SurfVector(x_ - other.x_, y_ - other.y_); }
    SurfVector operator+=(const SurfVector &other) {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }
    SurfVector operator-=(const SurfVector &other) {
        x_ -= other.x_;
        y_ -= other.y_;
        return *this;
    }
    SurfVector operator*(const T scalar) const { return SurfVector(x_ * scalar, y_ * scalar); }
    SurfVector operator/(const T scalar) const { return SurfVector(x_ / scalar, y_ / scalar); }
    T operator*(const SurfVector &other) const { return x_ * other.x_ + y_ * other.y_; }

    T x() const { return x_;}
    T y() const { return y_;}
    void set_x(T _x) { x_ = _x;}
    void set_y(T _y) { y_ = _y;}

    bool operator==(const SurfVector &other) const { return x_ == other.x_ && y_ == other.y_; }
    bool operator!=(const SurfVector &other) const { return x_ != other.x_ || y_ != other.y_; }
    friend std::ostream& operator<<(std::ostream& out, const SurfVector &v) {
        out << "(" << v.x_ << ", " << v.y_ << ")";
        return out;
    }
    friend std::istream& operator>>(std::istream& in, SurfVector &v) {
        in >> v.x_ >> v.y_;
        return in;
    }
};