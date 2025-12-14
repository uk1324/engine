#pragma once

#include <sstream>
#include <Types.hpp>

template<i32 N>
struct FixedSizeStringStream : std::ostream {
	struct Buffer : public std::stringbuf {
		int_type overflow(int_type c) override;
		char buffer[N];
		i32 currentSize = 0;
	};
	FixedSizeStringStream();

	Buffer buffer;

	const char* str();
};

template<i32 N>
inline FixedSizeStringStream<N>::FixedSizeStringStream()
	: std::ostream(&buffer) {
	buffer.buffer[0] = '\0';
}

template<i32 N>
const char* FixedSizeStringStream<N>::str() {
	return buffer.buffer;
}

template<i32 N>
FixedSizeStringStream<N>::int_type FixedSizeStringStream<N>::Buffer::overflow(int_type c) {
	if (currentSize < N - 1) {
		buffer[currentSize] = c;
		buffer[currentSize + 1] = '\0';
		currentSize++;
	}
	return 0;
}