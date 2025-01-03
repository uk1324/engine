#include "FileIo.hpp"
#include <engine/Log.hpp>
#include <fstream>

//std::string stringFromFile(std::string_view path) {
//	auto optString = tryLoadStringFromFile(path);
//	if (optString.has_value())
//		return std::move(*optString);
//
//	LOG_FATAL("couldn't read file \"%s\"", path.data());
//}

std::optional<std::string> tryLoadStringFromFile(std::string_view path) {
	std::ifstream file(path.data(), std::ios::binary);

	auto printFileReadError = [&]() {
		std::cerr << "Failed to read file '" << path << "' | " << strerror(errno);;
	};

	if (file.fail()) {
		printFileReadError();
		return std::nullopt;
	}

	auto start = file.tellg();
	file.seekg(0, std::ios::end);
	auto end = file.tellg();
	file.seekg(start);
	auto fileSize = end - start;

	std::string result;
	// Pointless memset
	result.resize(fileSize);

	file.read(result.data(), fileSize);
	if (file.fail()) {
		printFileReadError();
		return std::nullopt;
	}

	return result;
}

//Json::Value jsonFromFile(std::string_view path) {
//	auto optJson = tryLoadJsonFromFile(path);
//	if (optJson.has_value()) {
//		return std::move(*optJson);
//	}
//	LOG_FATAL("couldn't parse json file \"%s\"", path.data());
//	return Json::Value::null();
//}

//std::optional<Json::Value> tryLoadJsonFromFile(std::string_view path) {
//	const auto string = tryLoadJsonFromFile(path);
//	if (!string.has_value()) {
//		return std::nullopt;
//	}
//	try {
//		return Json::parse(*string);
//	} catch (const Json::ParsingError&) {
//		return std::nullopt;
//	}
//}
