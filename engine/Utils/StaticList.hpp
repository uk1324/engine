#pragma once

#include <Types.hpp>

template<typename T, size_t SIZE>
struct StaticList {

	StaticList();
	StaticList(const StaticList& other);

	void add(const T& v);
	//bool tryAdd(const T& v);

	T& operator[](size_t i);
	const T& operator[](size_t i) const;

	T* data();
	const T* data() const;
	i64 size() const;

	T* begin();
	T* end();
	const T* begin() const;
	const T* end() const;
	const T* cbegin() const;
	const T* cend() const;

	i64 size_;
	alignas(T) uint8_t data_[SIZE * sizeof(T)];
};

template<typename T, size_t SIZE>
StaticList<T, SIZE>::StaticList()
 : size_(0) {}

template<typename T, size_t SIZE>
StaticList<T, SIZE>::StaticList(const StaticList& other) 
	: size_(other.size_) {
	for (i64 i = 0; i < size_; i++) {
		new (&data()[i]) T(other.data()[i]);
	}
}

template<typename T, size_t SIZE>
void StaticList<T, SIZE>::add(const T& v) {
	new (&data()[size_]) T(v);
	size_++;
}

template<typename T, size_t SIZE>
T& StaticList<T, SIZE>::operator[](size_t i) {
	return data()[i];
}

template<typename T, size_t SIZE>
const T& StaticList<T, SIZE>::operator[](size_t i) const {
	return data()[i];
}

template<typename T, size_t SIZE>
T* StaticList<T, SIZE>::data() {
	return reinterpret_cast<T*>(data_);
}

template<typename T, size_t SIZE>
const T* StaticList<T, SIZE>::data() const {
	return reinterpret_cast<const T*>(data_);
}

template<typename T, size_t SIZE>
i64 StaticList<T, SIZE>::size() const {
	return size_;
}

template<typename T, size_t SIZE>
T* StaticList<T, SIZE>::begin() {
	return data();
}

template<typename T, size_t SIZE>
T* StaticList<T, SIZE>::end() {
	return data() +	size_;
}

template<typename T, size_t SIZE>
const T* StaticList<T, SIZE>::begin() const {
	return data();
}

template<typename T, size_t SIZE>
const T* StaticList<T, SIZE>::end() const {
	return data() + size_;
}

template<typename T, size_t SIZE>
const T* StaticList<T, SIZE>::cbegin() const {
	return data();
}

template<typename T, size_t SIZE>
const T* StaticList<T, SIZE>::cend() const {
	return data() + size_;
}
