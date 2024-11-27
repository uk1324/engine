#include "Json.hpp"

template<>
Vec2 fromJson<Vec2>(const Json::Value& json) {
	return Vec2{
		json.at(0).number(),
		json.at(1).number(),
	};
}

Json::Value toJson(const Vec2& value) {
	auto json = Json::Value::emptyArray();
	auto& array = json.array();
	array.push_back(value.x);
	array.push_back(value.y);
	return json;
}

Json::Value toJson(const Vec2T<int>& value) {
	auto json = Json::Value::emptyObject();
	json["x"] = Json::Value::IntType(value.x);
	json["y"] = Json::Value::IntType(value.y);
	return json;
}

template<>
Vec2T<int> fromJson<Vec2T<int>>(const Json::Value& json) {
	return Vec2T<int>{
		int(json.at("x").intNumber()),
		int(json.at("y").intNumber()),
	};
}

template<>
Vec3 fromJson<Vec3>(const Json::Value& json) {
	return Vec3(
		json.at(0).number(),
		json.at(1).number(),
		json.at(2).number()
	);
}

Json::Value toJson(const Vec3& value) {
	auto json = Json::Value::emptyArray();
	auto& array = json.array();
	array.push_back(value.x);
	array.push_back(value.y);
	array.push_back(value.z);
	return json;
}

template<>
Vec4 fromJson<Vec4>(const Json::Value& json) {
	return Vec4{
		json.at("x").number(),
		json.at("y").number(),
		json.at("z").number(),
		json.at("w").number(),
	};
}

Json::Value toJson(const Vec4& value) {
	auto json = Json::Value::emptyObject();
	json["x"] = value.x;
	json["y"] = value.y;
	json["z"] = value.z;
	json["w"] = value.w;
	return json;
}

template<>
std::string fromJson<std::string>(const Json::Value& json) {
	return json.string();
}

template<>
i32 fromJson<i32>(const Json::Value& json) {
	return i32(json.intNumber());
}

Json::Value toJson(i32 value) {
	return Json::Value(Json::Value::IntType(value));
}

Json::Value toJson(const std::string& value) {
	return Json::Value(value);
}
