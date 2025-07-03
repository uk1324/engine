#pragma once
#include <ostream>
#include <vector>
#include <sstream>
#include <Types.hpp>

struct SourceGenerator : std::ostream {
	struct IndentationScope {
		IndentationScope(SourceGenerator& generator);
		~IndentationScope() {
			generator.indentationEnd();
		}

		SourceGenerator& generator;
	};

	struct SourceGeneratorBuf : public std::stringbuf {
		SourceGeneratorBuf(SourceGenerator& self) : self(self) {}

		int_type overflow(int_type c) override;
		std::string string;
		SourceGenerator& self;
	};
	SourceGeneratorBuf buffer;

	[[nodiscard]] IndentationScope indentationScope();

	template<typename T>
	void separatedList(const std::vector<T>& v, const char* separator, const char* start, const char* end);

	SourceGenerator()
		: std::ostream(&buffer)
		, buffer(*this) {}

	std::string& string();
	//const std::string& string() const;

	i32 indentation = 0;
	void indentationStart();
	void indentationEnd();
};

template<typename T>
void SourceGenerator::separatedList(const std::vector<T>& v, const char* separator, const char* start, const char* end) {
	auto& g = *this;
	g << start;
	for (i32 i = 0; i < i32(v.size()) - 1; i++) {
		g << v[i] << separator;
	}
	if (v.size() > 0) {
		g << v[v.size() - 1];
	}
	g << end;
}
