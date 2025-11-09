#pragma once

#include <cmath>
#include "Vec3.hpp"
#include "Types.hpp"
#include "Assertions.hpp"

template<typename T>
struct Vec4T {
	Vec4T();
	explicit constexpr Vec4T(const T& v);
	constexpr Vec4T(const T& x, const T& y, const T& z, const T& w = 1);
	explicit constexpr Vec4T(Vec3T<T> v, const T& w = 1);

	Vec4T operator+(const Vec4T<T>& v) const;
	void operator+=(const Vec4T<T>& v);
	Vec4T operator-(const Vec4T<T>& v) const;
	void operator-=(const Vec4T<T>& v);
	Vec4T operator*(const T& s) const;
	void operator*=(const T& s);
	Vec4T operator/(const T& s) const;
	Vec4T& operator/=(const T& s);
	Vec4T operator-() const;

	bool operator==(const Vec4T& other) const = default;

	T& operator[](i32 i);
	const T& operator[](i32 i) const;

	Vec4T min(Vec4T v) const;
	Vec4T max(Vec4T v) const;

	T lengthSquared() const;
	T length() const;
	Vec4T<T> normalized() const;

	Vec3T<T> xyz() const;

	T* data();
	const T* data() const;
	T* begin();
	T* end();
	const T* cbegin() const;
	const T* cend() const;

	T x, y, z, w;
};

using Vec4 = Vec4T<float>;

template<typename T>
Vec4T<T> operator*(const T& s, const Vec4T<T>& v);

template<typename T>
T dot(const Vec4T<T>& a, const Vec4T<T>& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template<typename T>
inline Vec4T<T>::Vec4T()
	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
	, w(0.0f) {}

template<typename T>
constexpr Vec4T<T>::Vec4T(const T& v)
	: x(v)
	, y(v)
	, z(v)
	, w(0.0f) {}

template<typename T>
constexpr Vec4T<T>::Vec4T(const T& x, const T& y, const T& z, const T& w)
	: x(x)
	, y(y)
	, z(z)
	, w(w) {}

template<typename T>
constexpr Vec4T<T>::Vec4T(Vec3T<T> v, const T& w) 
	: x(v.x)
	, y(v.y)
	, z(v.z)
	, w(w) {}

template<typename T>
Vec4T<T> Vec4T<T>::operator+(const Vec4T<T>& v) const {
	return Vec4T(x + v.x, y + v.y, z + v.z, w + v.w);
}

template<typename T>
void Vec4T<T>::operator+=(const Vec4T<T>& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

template<typename T>
Vec4T<T> Vec4T<T>::operator-(const Vec4T<T>& v) const {
	return Vec4T(x - v.x, y - v.y, z - v.z, w - v.w);
}

template<typename T>
void Vec4T<T>::operator-=(const Vec4T<T>& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

template<typename T>
Vec4T<T> Vec4T<T>::operator*(const T& s) const {
	return Vec4T(x * s, y * s, z * s, w * s);
}

template<typename T>
void Vec4T<T>::operator*=(const T& s) {
	*this = *this * s;
}


template<typename T>
Vec4T<T> Vec4T<T>::operator/(const T& s) const {
	return Vec4T(x / s, y / s, z / s, w / s);
}

template<typename T>
Vec4T<T>& Vec4T<T>::operator/=(const T& s) {
	*this = *this / s;
	return *this;
}

template<typename T>
Vec4T<T> Vec4T<T>::operator-() const {
	return Vec4(-x, -y, -z, -w);
}

template<typename T>
T& Vec4T<T>::operator[](i32 i) {
	CHECK(i >= 0 && i <= 3);
	return data()[i];
}

template<typename T>
const T& Vec4T<T>::operator[](i32 i) const {
	return const_cast<Vec4T<float>*>(this)->operator[](i);
}

template<typename T>
Vec4T<T> Vec4T<T>::min(Vec4T v) const {
	return Vec4(
		std::min(x, v.x), 
		std::min(y, v.y),
		std::min(z, v.z),
		std::min(w, v.w)
	);
}

template<typename T>
Vec4T<T> Vec4T<T>::max(Vec4T v) const {
	return Vec4(
		std::max(x, v.x),
		std::max(y, v.y),
		std::max(z, v.z),
		std::max(w, v.w)
	);
}

template<typename T>
T Vec4T<T>::lengthSquared() const {
	return x * x + y * y + z * z + w * w;
}

template<typename T>
T Vec4T<T>::length() const {
	return sqrt(lengthSquared());
}

template<typename T>
Vec4T<T> Vec4T<T>::normalized() const {
	const auto l = length();
	if (l == 0.0f) {
		return *this;
	}
	return *this / length();
}

template<typename T>
Vec3T<T> Vec4T<T>::xyz() const {
	return Vec3T(x, y, z);
}

template<typename T>
T* Vec4T<T>::data() {
	return &x;
}

template<typename T>
inline const T* Vec4T<T>::data() const {
	return &x;
}

template<typename T>
T* Vec4T<T>::begin() {
	return data();
}

template<typename T>
T* Vec4T<T>::end() {
	return data() + 4;
}

template<typename T>
const T* Vec4T<T>::cbegin() const{
	return data();
}

template<typename T>
const T* Vec4T<T>::cend() const {
	return data() + 4;
}

template<typename T>
Vec4T<T> operator*(const T& s, const Vec4T<T>& v) {
	return v * s;
}
