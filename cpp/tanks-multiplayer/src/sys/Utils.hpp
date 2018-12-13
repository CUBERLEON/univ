#ifndef UTILS_HPP
#define UTILS_HPP

#include <utility>
#include <string>
#include <cmath>
#include <algorithm>

#define sqr(a) (a)*(a)

using std::pair;

const float EPS = 1e-3;
const float PI = acos(-1.);

template <typename T, typename U> pair<T, U> operator+ (const pair<T, U>& l, const pair<T, U>& r) { return{ l.first + r.first, l.second + r.second }; }
template <typename T, typename U> pair<T, U> operator+= (pair<T, U>& l, const pair<T, U>& r) { l.first += r.first; l.second += r.second; return l; }
template <typename T, typename U> pair<T, U> operator- (const pair<T, U>& l, const pair<T, U>& r) { return{ l.first - r.first, l.second - r.second }; }
template <typename T, typename U> pair<T, U> operator-= (pair<T, U>& l, const pair<T, U>& r) { l.first -= r.first; l.second -= r.second; return l; }
template <typename T, typename U> pair<T, U> operator* (const pair<T, U>& l, const float r) { return{ l.first * r, l.second * r }; }
template <typename T, typename U> pair<T, U> operator* (const float l, const pair<T, U>& r) { return{ r.first * l, r.second * l }; }
template <typename T, typename U> pair<T, U> operator*= (pair<T, U>& l, const float r) { l.first *= r; l.second *= r; return l; }
template <typename T, typename U> pair<T, U> operator/ (const pair<T, U>& l, const float r) { return{ l.first / r, l.second / r }; }
template <typename T, typename U> pair<T, U> operator/= (pair<T, U>& l, const float r) { l.first /= r; l.second /= r; return l; }

template <typename T, typename U>
float length(const pair<T, U>& a) {
    return std::sqrt(sqr(a.first) + sqr(a.second));
}

template <typename T, typename U>
float cross(const pair<T, U>& a, const pair<T, U>& b) {
    return a.first * b.second - a.second * b.first;
}

template <typename T, typename U>
pair<T, U> rotate(const pair<T, U>& a, float angle) {
    double c = cos(angle), s = sin(angle);
    return { c * a.first + s * a.second, -s * a.first + c * a.second };
}

template <typename T, typename U>
pair<T, U> scale(const pair<T, U>& a, const pair<T, U>& b) {
    return {a.first * b.first, a.second * b.second};
}
template <typename T, typename U>
pair<T, U> scale(const pair<T, U>& a, float scale) {
    return { scale * a.first, scale * a.second };
}

template <typename T>
int sign(T a) {
    return (a > 0) - (a < 0);
}

template <typename T, typename U>
int intersecting(const pair<T, U>& a1, const pair<T, U>& a2, const pair<T, U>& b1, const pair<T, U>& b2) {
    if ((b2.first - a1.first) * (b1.first - a2.first) > 0 ||
        (b2.second - a1.second) * (b1.second - a2.second) > 0)
        return 0;
    int s1 = sign(cross(a2-a1, b2-a1)) * sign(cross(a2-a1, b1-a1)),
        s2 = sign(cross(b2-b1, a2-b1)) * sign(cross(b2-b1, a1-b1));
    if (s1 < 0 && s2 < 0) return 2;
    if (s1 == 0 || s2 == 0) return 1;
    return 0;
}

void replaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);

#endif