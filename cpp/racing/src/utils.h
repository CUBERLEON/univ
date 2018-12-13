#ifndef UTILS_H
#define UTILS_H

#include <utility>
#include <string>
#include <cmath>

#define sqr(a) (a)*(a)

using std::pair;

const float EPS = 1e-3;

template <typename T, typename U> pair<T, U> operator+ (const pair<T, U>& l, const pair<T, U>& r) { return{ l.first + r.first, l.second + r.second }; }
template <typename T, typename U> pair<T, U> operator+= (pair<T, U>& l, const pair<T, U>& r) { l.first += r.first; l.second += r.second; return l; }
template <typename T, typename U> pair<T, U> operator- (const pair<T, U>& l, const pair<T, U>& r) { return{ l.first - r.first, l.second - r.second }; }
template <typename T, typename U> pair<T, U> operator-= (pair<T, U>& l, const pair<T, U>& r) { l.first -= r.first; l.second -= r.second; return l; }
template <typename T, typename U> pair<T, U> operator* (const pair<T, U>& l, const float r) { return{ l.first * r, l.second * r }; }
template <typename T, typename U> pair<T, U> operator* (const float l, const pair<T, U>& r) { return{ r.first * l, r.second * l }; }
template <typename T, typename U> pair<T, U> operator*= (pair<T, U>& l, const float r) { l.first *= r; l.second *= r; return l; }
template <typename T, typename U> pair<T, U> operator/ (const pair<T, U>& l, const float r) { return{ l.first / r, l.second / r }; }
template <typename T, typename U> pair<T, U> operator/= (pair<T, U>& l, const float r) { l.first /= r; l.second /= r; return l; }

template <typename T, typename U> float length(const pair<T, U>& a) {
    return std::sqrt(sqr(a.first) + sqr(a.second));
}

void replaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);

#endif