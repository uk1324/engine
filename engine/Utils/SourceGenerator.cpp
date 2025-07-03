#include "SourceGenerator.hpp"

SourceGenerator::IndentationScope::IndentationScope(SourceGenerator& generator)
	: generator(generator) {
	generator.indentationStart();
}

std::stringbuf::int_type SourceGenerator::SourceGeneratorBuf::overflow(int_type c) {
	if (c == '\n') {
		for (i32 i = 0; i < self.indentation; i++) {
			string += '\t';
		}
	}
	string += c;

	// Not sure what should be returned here. https://en.cppreference.com/w/cpp/io/basic_streambuf/overflow
	return 0;
}

SourceGenerator::IndentationScope SourceGenerator::indentationScope() {
	return IndentationScope(*this);
}

std::string& SourceGenerator::string() {
	return buffer.string;
}

void SourceGenerator::indentationStart() {
	indentation++;
}

void SourceGenerator::indentationEnd() {
	indentation--;
}
