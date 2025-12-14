#pragma once

#include <engine/Graphics/ShaderProgram.hpp>
#include <engine/Graphics/Texture.hpp>
#include "FontData.hpp"
#include <string_view>
#include <expected>

static constexpr std::pair<char32_t, char32_t> ASCII_CHARACTER_RANGES[]{
	{ 0, 127 },
};

static constexpr std::pair<char32_t, char32_t> NUMBERS_CHARACTER_RANGES[]{
	{ '0', '9' },
};

// https://character-table.netlify.app/polish/#unicode-ranges
static constexpr std::pair<char32_t, char32_t> POLISH_CHARACTER_RANGES[]{
	{ 0, 127 },
	{ 0x20, 0x5F },
	{ 0x61, 0x70 },
	{ 0x72, 0x75 },
	{ 0x77, 0x77 },
	{ 0x79, 0x7E },
	{ 0xA0, 0xA0 },
	{ 0xA7, 0xA7 },
	{ 0xA9, 0xA9 },
	{ 0xAB, 0xAB },
	{ 0xB0, 0xB0 },
	{ 0xBB, 0xBB },
	{ 0xD3, 0xD3 },
	{ 0xF3, 0xF3 },
	{ 0x104, 0x107 },
	{ 0x118, 0x119 },
	{ 0x141, 0x144 },
	{ 0x15A, 0x15B },
	{ 0x179, 0x17C },
	{ 0x2010, 0x2011 },
	{ 0x2013, 0x2014 },
	{ 0x201D, 0x201E },
	{ 0x2020, 0x2021 },
	{ 0x2026, 0x2026 },
	{ 0x2030, 0x2030 },
	{ 0x2032, 0x2033 },
	{ 0x20AC, 0x20AC },
};

struct TextInfo {
	Vec2 size;
	float bottomY;
};

struct Font {
	struct LoadError {
		std::string message;
	};

	static Font loadSdfWithCachingAtDefaultPath(std::string_view directory, std::string_view fontName, std::string_view extension = "ttf", std::span<const std::pair<char32_t, char32_t>> rangesToLoad = std::span(ASCII_CHARACTER_RANGES));
	TextInfo textInfo(float maxHeight, std::string_view text, f32 additionalSpacing = 0.0f) const;

	int pixelHeight;
	Texture fontAtlas;
	Vec2T<int> fontAtlasPixelSize;
	std::unordered_map<char32_t, Glyph> glyphs;
};

struct TextRenderInfoIterator {
	TextRenderInfoIterator(
		const Font& font,
		Vec2 pos, 
		const Mat3x2& transform,
		float maxHeight,
		std::string_view text,
		f32 additionalSpacing = 0.0f);

	const Font& font;
	Vec2 pos;
	const Mat3x2& transform;
	float maxHeight;
	std::string_view text;
	f32 additionalSpacing;

	const char* currentInText;

	struct CharacterRenderInfo {
		Mat3x2 transform;
		Vec2 offsetInAtlas;
		Vec2 sizeInAtlas;
		i32 indexInText;
	};
	std::optional<CharacterRenderInfo> characterRenderInfo(char32_t character);

	std::optional<CharacterRenderInfo> next();
};

// The font info is cached so when it changes it knows it needs to reload the sdf. It might be simpler to just store the ranges and read the info from the font file. Haven't though about it much it might not work idk.
std::expected<Font, Font::LoadError> fontLoadSdfWithCaching(
	const char* fontPath,
	const char* cachedSdfPath,
	const char* cachedFontInfoPath,
	std::span<const std::pair<char32_t, char32_t>> rangesToLoad,
	int fontPixelHeight);

Font loadFontSdfFromMemory(
	i32 pixelHeight, std::unordered_map<char32_t, Glyph>&& glyphs,
	const u8* image, i32 imageSizeX, i32 imageSizeY);
void saveFontToCpp(const char* imagePath, const char* fontDataPath);