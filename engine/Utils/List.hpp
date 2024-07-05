#pragma once

#include <Types.hpp>
#include <utility>

// This was created, because I had some issues with std::vector.
// std::vector is default initialized (has default constructor). This created problems in the level loading of my level editor. When I used struct initialization the values were default initialized so when adding a new array to the level struct it was easy to forget copy into the level struct (I guess you could make a function that copies the values into a json object instead of copying them into a struct and only then creating the json object, this wouldn't fix the forgetting to add it to the struct). In this case instead of using struct inititalization could have used a constructor. In this case you would need to remeber to add the field into the constructor so that wouldn't fix it compltly, but it would make it harder to forget to do it.
// std::vector is default copied. I want to try to write code that has to explicitly copy object. I had some issues with copying object in the past for example when an object was getting passed to another thread it was accidentally copied instead of moved which was causing perofrmance issues.
// std::vector has range checks in debug mode. This can sometimes slow thing too much. I guess you could get around it by using .data(). In this class I have the access to the implementation so I can make a method like uncheckedGet.
// std::vector uses size_t for size. size_t is unsigned so it wraps around, so you need to convert it to a signed time in some cases. If you forget you get a bug. I guess there is an advantage to using size_t. The wrap around makes it crash instantly in the case where there is a negative index accessed.

template<typename T>
struct List {
	static [[nodiscard]] List uninitialized(i64 size);
	static [[nodiscard]] List empty();

	List(const List&) = delete;
	List(List&& other) noexcept;
	~List();

	void add(const T& value);

	List clone();

	T& operator[](i64 index);
	const T& operator[](i64 index) const;

	List& operator=(const List&) = delete;
	List& operator=(List&& other) noexcept;

	T* begin();
	T* end();

	i64 size() const;
	T* data();
	const T* data() const;

private:
	//List(i64 size);
	List(i64 size, i64 capacity, T* data);

	T* data_;
	i64 size_;
	i64 capacity_;
};

template<typename T>
List<T> List<T>::uninitialized(i64 size) {
	return List(size, size, reinterpret_cast<T*>(operator new(sizeof(T) * size)));
}

template<typename T>
List<T> List<T>::empty() {
	return List(0, 0, nullptr);
}

template<typename T>
List<T>::~List() {
	operator delete(data_);
}

template<typename T>
void List<T>::add(const T& value) {
	//if (size + 1 > capacity)
	//{
	//	auto oldData = data;
	//	capacity = (capacity == 0) ? 8 : capacity * 2;
	//	data = reinterpret_cast<Value*>(::operator new(sizeof(Value) * capacity));
	//	memcpy(data, oldData, sizeof(Value) * size);
	//}

	//data[size] = value;
	//size++;

	if (size_ + 1 > capacity_) {
		if (capacity_ == 0) {
			capacity_ = 8;
		} else {
			capacity_ *= 2;
		}
		auto newData = reinterpret_cast<T*>(operator new(capacity_ * sizeof(T)));
		
		for (size_t i = 0; i < size_; i++) {
			new (&newData[i]) T(std::move(data_[i]));
			// TODO: Does the old data need to be destroyed?
		}
		
		data_ = newData;
	}
	new (&data_[size_]) T(value);
	size_++;
}

template<typename T>
List<T> List<T>::clone() {
	auto cloneData = reinterpret_cast<T*>(operator new(capacity_ * sizeof(T)));
	for (i64 i = 0; i < size_; i++) {
		new (&cloneData[i]) T(data_[i]);
	}
	return List(size_, capacity_, cloneData);
}

template<typename T>
T& List<T>::operator[](i64 index) {
	return data_[index];
}
//const T& operator[](i64 index) const;

template<typename T>
List<T>::List(List&& other) noexcept 
	: size_(other.size_)
	, data_(other.data_)
	, capacity_(other.capacity_) {
	other.data_ = nullptr;
	other.size_ = 0;
	other.capacity_ = 0;
}

template<typename T>
List<T>& List<T>::operator=(List&& other) noexcept {
	operator delete (data_);
	data_ = other.data_;
	size_ = other.size_;
	capacity_ = other.capacity_;
	other.data_ = nullptr;
	other.size_ = 0;
	other.capacity_ = 0;
}

template<typename T>
T* List<T>::begin() {
	return data_;
}

template<typename T>
T* List<T>::end() {
	return data_ + size_;
}

template<typename T>
i64 List<T>::size() const {
	return size_;
}

template<typename T>
T* List<T>::data() {
	return data_;
}

template<typename T>
const T* List<T>::data() const {
	return data_;
}

template<typename T>
List<T>::List(i64 size, i64 capacity, T* data)
	: size_(size)
	, capacity_(capacity)
	, data_(data) {}