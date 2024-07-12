#pragma once

#include <Types.hpp>
#include <engine/Math/Vec2.hpp>
#include "View2d.hpp"

template<typename T>
struct Array2d {
	static Array2d uninitialized(i64 sizeX, i64 sizeY);
	static Array2d uninitialized(Vec2T<i64> size);
	static Array2d filled(i64 sizeX, i64 sizeY, const T& value);
	static Array2d filled(Vec2T<i64> size, const T& value);
	Array2d(const Array2d&) = delete;
	Array2d(Array2d&& other) noexcept;
	~Array2d();

	Array2d& operator=(const Array2d&) = delete;
	Array2d& operator=(Array2d&&) noexcept;

	Array2d clone() const;

	T& operator()(i64 x, i64 y);
	const T& operator()(i64 x, i64 y) const;

	i64 sizeX() const;
	i64 sizeY() const;
	Vec2T<i64> size() const;
	T* data();
	const T* data() const;

private:
	Array2d(i64 sizeX, i64 sizeY);

	i64 sizeX_;
	i64 sizeY_;
	T* data_;
};

template<typename T>
View2d<T> view2d(Array2d<T>& a) {
	return View2d<T>(a.data(), a.sizeX(), a.sizeY());
}
template<typename T>
View2d<const T> constView2d(const Array2d<T>& a) {
	return View2d<const T>(a.data(), a.sizeX(), a.sizeY());
}

template<typename T>
Array2d<T> Array2d<T>::uninitialized(i64 sizeX, i64 sizeY) {
	return Array2d<T>(sizeX, sizeY);
}

template<typename T>
Array2d<T> Array2d<T>::uninitialized(Vec2T<i64> size) {
	return uninitialized(size.x, size.y);
}

template<typename T>
Array2d<T> Array2d<T>::filled(i64 sizeX, i64 sizeY, const T& value) {
	auto result = Array2d<T>::uninitialized(sizeX, sizeY);
	for (i64 y = 0; y < sizeY; y++) {
		for (i64 x = 0; x < sizeX; x++) {
			result(x, y) = value;
		}
	}
	return result;
}

template<typename T>
Array2d<T> Array2d<T>::filled(Vec2T<i64> size, const T& value) {
	return filled(size.x, size.y, value);
}

template<typename T>
Array2d<T>::Array2d(Array2d&& other) noexcept
	: sizeX_(other.sizeX_)
	, sizeY_(other.sizeY_)
	, data_(other.data_) {
	other.data_ = nullptr;
	other.sizeX_ = 0;
	other.sizeY_ = 0;
}

template<typename T>
Array2d<T>::~Array2d() {
	operator delete(data_);
}

template<typename T>
Array2d<T>& Array2d<T>::operator=(Array2d&& other) noexcept {
	operator delete(data_);
	sizeX_ = other.sizeX_;
	sizeY_ = other.sizeY_;
	data_ = other.data_;
	other.data_ = nullptr;
	other.sizeX_ = 0;
	other.sizeY_ = 0;
}

template<typename T>
Array2d<T> Array2d<T>::clone() const {
	auto m = Array2d<T>::uninitialized(size());
	memcpy(m.data(), data_, sizeX_ * sizeY_ * sizeof(T));
	return m;
}

template<typename T>
T& Array2d<T>::operator()(i64 x, i64 y) {
	DEBUG_ASSERT(x >= 0 && x < sizeX_);
	DEBUG_ASSERT(y >= 0 && y < sizeY_);
	return data_[y * sizeX_ + x];
}

template<typename T>
const T& Array2d<T>::operator()(i64 x, i64 y) const {
	return const_cast<Array2d<T>*>(this)->operator()(x, y);
}

template<typename T>
i64 Array2d<T>::sizeX() const {
	return sizeX_;
}

template<typename T>
i64 Array2d<T>::sizeY() const {
	return sizeY_;
}

template<typename T>
Vec2T<i64> Array2d<T>::size() const {
	return Vec2T<i64>(sizeX_, sizeY_);
}

template<typename T>
T* Array2d<T>::data() {
	return data_;
}

template<typename T>
const T* Array2d<T>::data() const {
	return data_;
}

template<typename T>
Array2d<T>::Array2d(i64 sizeX, i64 sizeY)
	: sizeX_(sizeX)
	, sizeY_(sizeY) {
	data_ = reinterpret_cast<T*>(operator new(sizeX * sizeY * sizeof(T)));
}


//#pragma once
//
//#include <engine/Math/Vec2.hpp>
//#include <span>
//#include <Span2d.hpp>
//
//// For efficient cache use outer loop over Y inner loop over X.
//template<typename T>
//class Array2d {
//public:
//	Array2d();
//	Array2d(i64 x, i64 y);
//	Array2d(Vec2T<i64> size);
//	explicit Array2d(const Array2d<T>& other);
//	~Array2d();
//	Array2d(Array2d&& other) noexcept;
//	auto operator=(Array2d&& other) noexcept -> Array2d&;
//
//	static Array2d withAllSetTo(i64 x, i64 y, const T& value);
//
//	Array2d copy() const;
//
//	// using operator() for multidimensional indexing is better that using operator[], because it allows any way of structuring data. When using operator[] if you want the first index to be the x and the second the y then the data has to be laid you this way(unless you make a special type). It also may be simpler to write.
//	// The character count is the same.
//	// [x][y]
//	// (x, y)
//	auto operator()(i64 x, i64 y) -> T&;
//	auto operator()(i64 x, i64 y) const -> const T&;
//	// Not sure if using at instead of operator() makes much sense. I guess it might look weird if you just call and array on a single value.
//	auto at(Vec2T<i64> pos) -> T&;
//	auto at(Vec2T<i64> pos) const -> const T&;
//	T& atClamped(i64 x, i64 y);
//	const T& atClamped(i64 x, i64 y) const;
//
//	i64 clampX(i64 x) const;
//	i64 clampY(i64 y) const;
//
//	bool isInBounds(i64 x, i64 y) const;
//
//	void fillRectSize(i64 minX, i64 minY, i64 width, i64 height, const T& value);
//	void paste(
//		i64 offsetInThisX, 
//		i64 offsetInThisY,
//		const Array2d<T>& pasted, 
//		i64 offsetInPastedX = 0, 
//		i64 offsetInPastedY = 0);
//
//	auto transpose() -> void;
//
//	auto cellCount() const -> i64;
//private:
//	Vec2T<i64> size_;
//	T* data_;
//public:
//	auto size() const -> Vec2T<i64> { return size_; }
//	auto data() const -> T* { return data_; }
//	i64 dataBytesSize() const { return cellCount() * sizeof(T); }
//	std::span<T> span();
//	std::span<const T> span() const;
//	Span2d<T> span2d();
//	Span2d<const T> span2d() const;
//};
//
//template<typename T>
//Array2d<T>::Array2d()
//	: data_{ nullptr }
//	, size_{ 0, 0 } {}
//
//template<typename T>
//Array2d<T>::Array2d(i64 x, i64 y)
//	: data_{ reinterpret_cast<T*>(operator new(x * y * sizeof(T))) }
//	, size_{ Vec2T<i64>{ x, y } } {}
//
//template<typename T>
//Array2d<T>::Array2d(Vec2T<i64> size)
//	: Array2d<T>{ size.x, size.y } {}
//
//template<typename T>
//Array2d<T>::Array2d(const Array2d<T>& other)
//	: Array2d(other.size()) {
//	for (i64 y = 0; y < size_.y; y++) {
//		for (i64 x = 0; x < size_.x; x++) {
//			operator()(x, y) = other(x, y);
//		}
//	}
//}
//
//template<typename T>
//Array2d<T>::~Array2d() {
//	operator delete(data_);
//}
//
//template<typename T>
//Array2d<T>::Array2d(Array2d&& other) noexcept
//	: data_{ other.data_ }
//	, size_{ other.size_ } {
//	other.data_ = nullptr;
//}
//
//template<typename T>
//auto Array2d<T>::operator=(Array2d&& other) noexcept -> Array2d& {
//	data_ = other.data_;
//	size_ = other.size_;
//	other.data_ = nullptr;
//	return *this;
//}
//
//template<typename T>
//Array2d<T> Array2d<T>::withAllSetTo(i64 x, i64 y, const T& value) {
//	Array2d r(x, y);
//	for (i64 y = 0; y < r.size_.y; y++) {
//		for (i64 x = 0; x < r.size_.x; x++) {
//			r(x, y) = value;
//		}
//	}
//	return r;
//}
//
//template<typename T>
//Array2d<T> Array2d<T>::copy() const {
//	ASSERT_NOT_REACHED();
//	return Array2d();
//}
//
//template<typename T>
//auto Array2d<T>::operator()(i64 x, i64 y) -> T& {
//	return *(data_ + y * size_.x + x);
//}
//
//template<typename T>
//auto Array2d<T>::operator()(i64 x, i64 y) const -> const T& {
//	return const_cast<Array2d*>(this)->operator()(x, y);
//}
//
//template<typename T>
//auto Array2d<T>::at(Vec2T<i64> pos) -> T& {
//	return operator()(pos.x, pos.y);
//}
//
//template<typename T>
//auto Array2d<T>::at(Vec2T<i64> pos) const -> const T& {
//	return operator()(pos.x, pos.y);
//}
//
//template<typename T>
//T& Array2d<T>::atClamped(i64 x, i64 y) {
//	return operator()(clampX(x), clampY(y));
//}
//
//template<typename T>
//const T& Array2d<T>::atClamped(i64 x, i64 y) const {
//	return const_cast<Array2d*>(this)->operator()(x, y);
//}
//
//template<typename T>
//i64 Array2d<T>::clampX(i64 x) const {
//	//// TODO: Test.
//	// Later: Seems to work
//	//ASSERT_NOT_REACHED();
//	return std::clamp(x, i64(0), size_.x);
//}
//
//template<typename T>
//i64 Array2d<T>::clampY(i64 y) const {
//	return std::clamp(y, i64(0), size_.y);
//}
//
//template<typename T>
//bool Array2d<T>::isInBounds(i64 x, i64 y) const {
//	return x >= 0 && y >= 0 && x < size_.x && y < size_.y;
//}
//
//template<typename T>
//void Array2d<T>::fillRectSize(i64 minX, i64 minY, i64 width, i64 height, const T& value) {
//	for (auto y = minY; y < minY + height; y++) {
//		for (auto x = minX; x < minX + width; x++) {
//			operator()(x, y) = value;
//		}
//	}
//}
//
//template<typename T>
//void Array2d<T>::paste(
//	i64 offsetInThisX,
//	i64 offsetInThisY,
//	const Array2d<T>& pasted,
//	i64 offsetInPastedX,
//	i64 offsetInPastedY) {
//
//	for (i32 yi = offsetInPastedY; yi < pasted.size().y; yi++) {
//		for (i32 xi = offsetInPastedX; xi < pasted.size().x; xi++) {
//			const auto thisXi = xi + offsetInThisX - offsetInPastedX;
//			const auto thisYi = yi + offsetInThisY - offsetInPastedY;
//			if (thisXi < 0 || thisXi >= size_.x || thisYi < 0 || thisYi >= size_.y) {
//				continue;
//			}
//
//			operator()(thisXi, thisYi) = pasted(xi, yi);
//		}
//	}
//}
//
//template<typename T>
//auto Array2d<T>::transpose() -> void {
//	if (size_.x != size_.y) {
//		std::swap(size_.x, size_.y);
//	} else {
//		ASSERT_NOT_REACHED();
//	}
//}
//
//template<typename T>
//auto Array2d<T>::cellCount() const -> i64 {
//	return size_.x * size_.y;
//}
//
//template<typename T>
//std::span<T> Array2d<T>::span() {
//	return std::span(data_, cellCount());
//}
//
//template<typename T>
//std::span<const T> Array2d<T>::span() const {
//	return std::span(data_, cellCount());
//}
//
//template<typename T>
//Span2d<T> Array2d<T>::span2d() {
//	return Span2d<T>(data_, size_.x, size_.y);
//}
//
//template<typename T>
//Span2d<const T> Array2d<T>::span2d() const {
//	return Span2d<const T>(data_, size_.x, size_.y);
//}