#pragma once

#include "Vec3.hpp"
#include <Assertions.hpp>

template<typename T>
struct Mat3T {
	constexpr Mat3T(const Vec3T<T>& x, const Vec3T<T>& y, const Vec3T<T>& z);
	Mat3T(const std::initializer_list<T>& data);

	static Mat3T rotationX(const T& angle);
	static Mat3T scale(const Vec3T<T>& s);
	static Mat3T scale(T x, T y, T z);
	static Mat3T scale(const T& s);
	static Mat3T rotationY(const T& angle);

	Mat3T operator*(const Mat3T& m) const;
	Mat3T transposed() const;

	const Vec3T<T>& operator[](i32 i) const;
	Vec3T<T>& operator[](i32 i);
	T& operator()(i32 i, i32 j);
	const T& operator()(i32 i, i32 j) const;

	const T* data() const;
	T* data();

	Vec3T<T> basis[3];

	static const Mat3T identity;
};

using Mat3 = Mat3T<float>;

template<typename T>
Vec3T<T> operator*(const Vec3T<T>& v, const Mat3T<T>& m);
template<typename T>
Vec3T<T>& operator*=(Vec3T<T>& v, const Mat3T<T>& m);
template<typename T>
Mat3T<T> operator*(f32 s, const Mat3T<T>& m);

template<typename T>
constexpr Mat3T<T>::Mat3T(const Vec3T<T>& x, const Vec3T<T>& y, const Vec3T<T>& z)
	: basis{ x, y, z } {}

/*
Mat3{ 
	0, 1, 2,
	3, 4, 5,
	6, 7, 8,
} -> Mat3 {
	Vec3T<T>(0, 3, 6),
	Vec3T<T>(1, 4, 5)
	Vec3T<T>(2, 5, 8)
}
*/
template<typename T>
Mat3T<T>::Mat3T(const std::initializer_list<T>& data)
	: basis{
		Vec3T<T>(data.begin()[0], data.begin()[3], data.begin()[6]),
		Vec3T<T>(data.begin()[1], data.begin()[4], data.begin()[7]),
		Vec3T<T>(data.begin()[2], data.begin()[5], data.begin()[8])
	} {
	CHECK(data.size() == 9);
}

template<typename T>
Mat3T<T> Mat3T<T>::rotationX(const T& angle) {
	const T c = cos(angle);
	const T s = sin(angle);
	return {
		{ 1, 0, 0 },
		{ 0, c, -s },
		{ 0, s, c },
	};
}

template<typename T>
Mat3T<T> Mat3T<T>::scale(const Vec3T<T>& s) {
	return {
		{ s.x, 0, 0 },
		{ 0, s.y, 0 },
		{ 0, 0, s.z },
	};
}

template<typename T>
Mat3T<T> Mat3T<T>::scale(T x, T y, T z) {
	return scale(Vec3(x, y, z));
}

template<typename T>
Mat3T<T> Mat3T<T>::scale(const T& s) {
	return {
		{ s, 0, 0 },
		{ 0, s, 0 },
		{ 0, 0, s },
	};
}

template<typename T>
Mat3T<T> Mat3T<T>::rotationY(const T& angle) {
	const T c = cos(angle);
	const T s = sin(angle);
	return {
		{ c, 0, s },
		{ 0, 1, 0 },
		{ -s, 0, c },
	};
}

template<typename T>
Mat3T<T> Mat3T<T>::operator*(const Mat3T& m) const {
	//return { basis[0] * m, basis[1] * m, basis[2] * m };
	return { m.basis[0] * *this, m.basis[1] * *this, m.basis[2] * *this };
}

template<typename T>
Mat3T<T> Mat3T<T>::transposed() const {
	const auto& m = *this;
	return Mat3T<T>{
		m(0, 0), m(0, 1), m(0, 2),
		m(1, 0), m(1, 1), m(1, 2),
		m(2, 0), m(2, 1), m(2, 2)
	};
}

template<typename T>
const Vec3T<T>& Mat3T<T>::operator[](i32 i) const {
	return const_cast<Mat3T<T>*>(this)->operator[](i);
}

template<typename T>
Vec3T<T>& Mat3T<T>::operator[](i32 i) {
	CHECK(i >= 0 && i <= 2);
	return basis[i];
}

template<typename T>
T& Mat3T<T>::operator()(i32 i, i32 j) {
	return data()[i * 3 + j];
}

template<typename T>
const T& Mat3T<T>::operator()(i32 i, i32 j) const {
	return const_cast<Mat3T<T>*>(this)->operator()(i, j);
}

template<typename T>
const T* Mat3T<T>::data() const {
	return reinterpret_cast<const float*>(basis);
}

template<typename T>
T* Mat3T<T>::data() {
	return reinterpret_cast<float*>(basis);
}

template<typename T>
Vec3T<T> operator*(const Vec3T<T>& v, const Mat3T<T>& m) {
	return v.x * m[0] + v.y * m[1] + v.z * m[2];
}

template<typename T>
Vec3T<T>& operator*=(Vec3T<T>& v, const Mat3T<T>& m) {
	v = v * m;
	return v;
}


template<typename T>
Mat3T<T> operator*(f32 s, const Mat3T<T>& m) {
	return Mat3T<T>(
		s * m[0],
		s * m[1],
		s * m[2]
	);
}

template<typename T>
const Mat3T<T> Mat3T<T>::identity(Vec3T<T>(1, 0, 0), Vec3T<T>(0, 1, 0), Vec3T<T>(0, 0, 1));