#pragma once

#include "Vec3.hpp"
#include "Mat3.hpp"

template<typename T>
struct QuatT {
	constexpr QuatT(const T& x, const T& y, const T& z, const T& w);
	QuatT(const T& angle, const Vec3T<T>& axis);

	QuatT<T> operator* (const QuatT<T>& rhs) const;
	QuatT<T> operator*= (const QuatT<T>& rhs);
	Vec3T<T> operator* (const Vec3T<T>& rhs) const;

	QuatT<T> operator/(f32 value) const;

	QuatT conjugate() const;
	// Rotations are by definition normalized.
	QuatT inverseIfNormalized() const;

	QuatT normalized() const;

	T length() const;

	Mat3T<T> toMatrix() const;

	const static QuatT identity;

	T x, y, z, w;
};

template<typename T>
Vec3T<T> operator*(const Vec3T<T>& lhs, const QuatT<T>& rhs);
template<typename T>
Vec3T<T>& operator*=(Vec3T<T>& lhs, const QuatT<T>& rhs);

using Quat = QuatT<float>;

template<typename T>
constexpr QuatT<T>::QuatT(const T& x, const T& y, const T& z, const T& w)
	: x(x)
	, y(y)
	, z(z)
	, w(w) {}

template<typename T>
QuatT<T>::QuatT(const T& angle, const Vec3T<T>& axis) {
	// TODO: Derive.
	T c = cos(angle / 2);
	T s = sin(angle / 2);
	w = c;
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
}

template<typename T>
QuatT<T> QuatT<T>::operator*(const QuatT<T>& rhs) const {
	return QuatT(y * rhs.z - z * rhs.y + x * rhs.w + w * rhs.x,
		z * rhs.x - x * rhs.z + y * rhs.w + w * rhs.y,
		x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z,
		w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
}

template<typename T>
QuatT<T> QuatT<T>::operator*=(const QuatT<T>& rhs) {
	*this = rhs * *this;
}

template<typename T>
Vec3T<T> QuatT<T>::operator*(const Vec3T<T>& rhs) const {
	// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
	QuatT<T> q = (*this * QuatT<T>(rhs.x, rhs.y, rhs.z, 0)) * conjugate();
	return Vec3T<T>(q.x, q.y, q.z);
}

template<typename T>
QuatT<T> QuatT<T>::operator/(f32 value) const {
	return QuatT<T>(x / value, y / value, z / value, w / value);
}

template<typename T>
QuatT<T> QuatT<T>::conjugate() const {
	return QuatT<T>(-x, -y, -z, w);
}

template<typename T>
QuatT<T> QuatT<T>::inverseIfNormalized() const {
	return conjugate();
}

template<typename T>
QuatT<T> QuatT<T>::normalized() const {
	return *this / length();
}

template<typename T>
T QuatT<T>::length() const {
	return std::sqrt(x * x + y * y + z * z + w * w);
}

template<typename T>
Mat3T<T> QuatT<T>::toMatrix() const {
	// TODO: This may need to be transposed.
	return Mat3T<T>(
		Vec3(1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w),
		Vec3(2 * x * y + 2 * z * w, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w),
		Vec3(2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y)
	);
}

template<typename T>
Vec3T<T> operator* (const Vec3T<T>& lhs, const QuatT<T>& rhs) {
	return rhs * lhs;
}

template<typename T>
Vec3T<T>& operator*= (Vec3T<T>& lhs, const QuatT<T>& rhs) {
	lhs = lhs * rhs;
	return lhs;
}

template<typename T>
const QuatT<T> QuatT<T>::identity = QuatT<T>(0, 0, 0, 1);