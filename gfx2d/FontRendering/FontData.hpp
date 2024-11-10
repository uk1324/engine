#pragma once
#include <Json.hpp> 

struct Glyph {
    Vec2T<int> offsetInAtlas = Vec2T<int>(0); 
    Vec2T<int> sizeInAtlas = Vec2T<int>(0); 
    Vec2T<int> visibleSize = Vec2T<int>(0); 
    Vec2T<int> bearingRelativeToOffsetInAtlas = Vec2T<int>(0); 
    Vec2T<int> visibleBearing = Vec2T<int>(0); 
    Vec2T<int> advance = Vec2T<int>(0); 

    	bool isVisible() const;
    	 
};

Json::Value toJson(const Glyph& value);
template<>
Glyph fromJson<Glyph>(const Json::Value& json);

 