#include "Font.hpp"
#include <engine/Graphics/TextureAtlasGenerator.hpp>
#include <Json/JsonPrinter.hpp>
#include <engine/Utils/JsonFileIo.hpp>
#include <dependencies/ft2build.h>
#include <engine/Utils/Json.hpp>
#include <Utf8.hpp>
#include FT_FREETYPE_H  
#include <gfx2d/Camera.hpp>
#include <filesystem>
#include <fstream>
#include <engine/Log.hpp>

Font Font::loadSdfWithCachingAtDefaultPath(std::string_view directory, std::string_view fontName, std::string_view extension) {
	const auto fontPath = std::string(directory) + "/" + std::string(fontName) + "." + std::string(extension);
	const auto atlasPath = "cached/" + std::string(fontName) + ".png";
	const auto infoPath = "cached/" + std::string(fontName) + ".json";

	auto font = fontLoadSdfWithCaching(
		fontPath.c_str(),
		atlasPath.c_str(),
		infoPath.c_str(),
		ASCII_CHARACTER_RANGES,
		64
	);
	//put("font loading took %", timer.elapsedMilliseconds());
	if (font.has_value()) {
		return std::move(*font);
	}
	Log::fatal("failed to load font: %", font.error().message.c_str());
}

TextInfo Font::textInfo(float maxHeight, std::string_view text, f32 additionalSpacing) const {
	const auto scale = maxHeight * (1.0f / pixelHeight);

	float width = 0.0f;
	float minY = std::numeric_limits<float>::infinity();
	float maxY = 0.0f;

	const char* current = text.data();
	const char* end = text.data() + text.size();
	while (const auto codepoint = Utf8::readCodePoint(current, end)) {
		current = codepoint->next;
		const auto& characterIt = glyphs.find(codepoint->codePoint);
		if (characterIt == glyphs.end()) {
			continue;
		}
		const auto& info = characterIt->second;
		const auto advance = (info.advance.x >> 6) * scale + additionalSpacing;
		width += advance;
		const auto bottomY = -(info.visibleSize.y - info.visibleBearing.y);
		const auto topY = bottomY + info.visibleSize.y;
		minY = std::min(minY, bottomY * scale);
		maxY = std::max(maxY, topY * scale);
	}
	// Don't have only add width istead of advance if it is the last character, because of how advance works.
	// In the glyph metrics image you can see that the character is offset from the origin.
	// https://learnopengl.com/In-Practice/Text-Rendering
	// This might not be correct, but I think it is.
	// You can test it works by for example writing "oo"

	float height = maxY - minY;
	if (isnan(height)) {
		height = 0.0f;
	}

	return TextInfo{
		.size = Vec2(width, height),
		.bottomY = minY
	};
}

Json::Value toJson(char32_t v) {
	return Json::Value(std::to_string(v));
}

template<>
char32_t fromJson<char32_t>(const Json::Value& json) {
	return std::stoi(json.string());
}

template<typename K, typename V>
static Json::Value mapToJson(const std::unordered_map<K, V>& map) {
	auto json = Json::Value::emptyArray();
	auto& jsonArray = json.array();
	// Maybe serialize std::pair.
	for (const auto& [key, value] : map) {
		auto object = Json::Value::emptyObject();
		object["key"] = toJson(key);
		object["value"] = toJson(value);
		jsonArray.push_back(object);
	}
	return json;
}

template<typename K, typename V>
static std::unordered_map<K, V> fromJson(const Json::Value& value) {
	std::unordered_map<K, V> result;
	for (const auto& pair : value.array()) {
		result[fromJson<K>(pair.at("key"))] = fromJson<V>(pair.at("value"));
	}
	return result;
}

#include <iostream>

std::expected<Font, Font::LoadError> fontLoadSdfWithCaching(
	const char* fontPath,
	const char* cachedSdfPath,
	const char* cachedFontInfoPath,
	std::span<const std::pair<char32_t, char32_t>> rangesToLoad,
	int fontPixelHeight) {
	const Texture::Settings atlasTextureSettings{
		.format = Texture::Format::RGBA,
		.wrapS = Texture::Wrap::CLAMP_TO_EDGE,
		.wrapT = Texture::Wrap::CLAMP_TO_EDGE,
	};

	auto tryLoadCached = [&]() -> std::optional<Font> {
		const auto cachedSdfImage = Image32::fromFile(cachedSdfPath);

		if (!cachedSdfImage.has_value()) {
			return std::nullopt;
		}

		const auto fontInfo = tryLoadJsonFromFile(cachedFontInfoPath);
		if (!fontInfo.has_value()) {
			return std::nullopt;
		}

		try {
			const auto cachedFontHeight = fontInfo->at("pixelHeight").intNumber();
			if (cachedFontHeight != fontPixelHeight) {
				return std::nullopt;
			}
			auto glyphs = fromJson<char32_t, Glyph>(fontInfo->at("glyphs"));

			for (const auto& range : rangesToLoad) {
				for (char32_t character = range.first; character <= range.second; character++) {
					if (glyphs.find(character) == glyphs.end()) {
						return std::nullopt;
					}
				}
			}

			return Font{
				.pixelHeight = i32(cachedFontHeight),
				.fontAtlas = Texture(*cachedSdfImage, atlasTextureSettings),
				.fontAtlasPixelSize = Vec2T<int>(cachedSdfImage->size()),
				.glyphs = std::move(glyphs)
			};
		} catch (const Json::Value::Exception&) {
			return std::nullopt;
		}
	};

	if (auto result = tryLoadCached()) {
		return std::move(*result);
	}

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		return std::unexpected(Font::LoadError{ "could not init FreeType Library" });
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath, 0, &face)) {
		return std::unexpected(Font::LoadError{ "failed to load font" });
	}

	FT_Set_Pixel_Sizes(face, 0, fontPixelHeight);

	std::unordered_map<char32_t, Glyph> glyphs;
	std::vector<TextureAtlasInputImage> textureAtlasInput;
	for (const auto& range : rangesToLoad) {
		// Ignore invalid ranges first > second
		for (char32_t character = range.first; character <= range.second; character++) {
			const auto glyphIndex = FT_Get_Char_Index(face, character);
			
			if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT)) {
				return std::unexpected(Font::LoadError{ 
					"failed to load glyph with index " + std::to_string(glyphIndex) + " = U+" + std::to_string(character)
				});
			}
			#ifdef WIN32
			// Fonts with overlapping parts don't render correctly. 
			// for example view "Orbitron" font using https://fontdrop.info/. The letters 'Q' and 'R' have overlaps and don't render correctly.
			// Freetype has 2 SDF renderers the sdf renderer and the bsdf renderer. To use the latter you have to first render the bitmap for example using FT_RENDER_MODE_NORMAL and the use FT_RENDER_MODE_SDF.
			// This is documented here: https://freetype.org/freetype2/docs/reference/ft2-glyph_retrieval.html.
			// search 'intersec'
			FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
			if (const auto error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF)) {
			#else 
			if (const auto error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) {
			#endif
				std::cout << error << '\n';
				return std::unexpected(Font::LoadError{ "failed to render glyph with index " + std::to_string(glyphIndex) });
			}

			const FT_Bitmap& bitmap = face->glyph->bitmap;

			
			glyphs[character] = Glyph{
				.offsetInAtlas = Vec2T<int>(0), // Set after the atlas is constructed
				.sizeInAtlas = { static_cast<int>(bitmap.width), static_cast<int>(bitmap.rows) },
				.visibleSize = { static_cast<int>(face->glyph->metrics.width) >> 6, static_cast<int>(face->glyph->metrics.height) >> 6  },
				.bearingRelativeToOffsetInAtlas = { face->glyph->bitmap_left, face->glyph->bitmap_top },
				.visibleBearing = { static_cast<int>(face->glyph->metrics.horiBearingX) >> 6, static_cast<int>(face->glyph->metrics.horiBearingY) >> 6  },
				.advance = { face->glyph->advance.x, face->glyph->advance.y },
			};

			if (bitmap.rows == 0 || bitmap.width == 0) {
				continue;
			}

			Image32 image(bitmap.width, bitmap.rows);

			for (unsigned int y = 0; y < bitmap.rows; y++) {
				for (unsigned int x = 0; x < bitmap.width; x++) {
					const auto value = bitmap.buffer[y * bitmap.pitch + x];
					image(x, y) = Pixel32{ value };
				}
			}

			const auto charBytes = reinterpret_cast<char*>(&character);
			// TODO: Maybe check for duplicates. Check if intersection between ranges maybe.
			textureAtlasInput.push_back(TextureAtlasInputImage{ 
				.name = std::string(charBytes, charBytes + sizeof(char32_t)), 
				.image = std::move(image) 
			});
		}
	}

	auto output = generateTextureAtlas(textureAtlasInput);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	Vec2T<int> fontAtlasPixelSize(output.atlasImage.size());

	for (auto& [character, info] : glyphs) {
		const auto charBytes = reinterpret_cast<const char*>(&character);
		const auto atlasInfo = output.nameToPos.find(std::string(charBytes, charBytes + sizeof(char32_t)));
		if (atlasInfo == output.nameToPos.end()) {
			continue;
		}

		info.offsetInAtlas = atlasInfo->second.offset;
	}

	auto info = Json::Value::emptyObject();
	info["glyphs"] = mapToJson(glyphs);
	info["pixelHeight"] = Json::Value::IntType(fontPixelHeight);

	auto createDirectories = [](const char* filePath) {
		using namespace std::filesystem;
		create_directories(path(filePath).parent_path());
	};

	createDirectories(cachedFontInfoPath);
	std::ofstream jsonFile(cachedFontInfoPath);
	Json::print(jsonFile, info);

	createDirectories(cachedSdfPath);
	output.atlasImage.saveToPng(cachedSdfPath);

	return Font{
		.pixelHeight = fontPixelHeight,
		.fontAtlas = Texture(output.atlasImage, atlasTextureSettings),
		.fontAtlasPixelSize = Vec2T<int>(output.atlasImage.size()),
		.glyphs = std::move(glyphs) 
	};
}

bool Glyph::isVisible() const {
	return visibleSize.x > 0 && visibleSize.y > 0;
}

TextRenderInfoIterator::TextRenderInfoIterator(
	const Font& font,
	Vec2 pos,
	const Mat3x2& transform,
	float maxHeight,
	std::string_view text,
	f32 additionalSpacing)
	: font(font)
	, pos(pos)
	, transform(transform)
	, maxHeight(maxHeight)
	, text(text) 
	, currentInText(text.data())
	, additionalSpacing(additionalSpacing) {

}

std::optional<TextRenderInfoIterator::CharacterRenderInfo> TextRenderInfoIterator::characterRenderInfo(char32_t character) {
	const auto& characterIt = font.glyphs.find(character);
	if (characterIt == font.glyphs.end()) {
		return std::nullopt;
	}
	const auto& info = characterIt->second;

	float scale = maxHeight * (1.0f / font.pixelHeight);
	auto centerPos = pos + Vec2(info.bearingRelativeToOffsetInAtlas.x, -(info.sizeInAtlas.y - info.bearingRelativeToOffsetInAtlas.y)) * scale;
	const auto size = Vec2(info.sizeInAtlas) * scale;
	centerPos += size / 2.0f;

	std::optional<CharacterRenderInfo> result;
	if (info.isVisible()) {
		const auto characterTransform = makeObjectTransform(centerPos, 0.0f, size / 2.0f) * transform;
		result = CharacterRenderInfo{
			.transform = characterTransform,
			.offsetInAtlas = Vec2(info.offsetInAtlas) / Vec2(font.fontAtlasPixelSize),
			.sizeInAtlas = Vec2(info.sizeInAtlas) / Vec2(font.fontAtlasPixelSize),
		};
	}

	// Advance values are stored as 1/64 of a pixel.
	pos = Vec2(pos.x + (info.advance.x >> 6) * scale + additionalSpacing, pos.y);
	return result;
}

std::optional<TextRenderInfoIterator::CharacterRenderInfo> TextRenderInfoIterator::next() {
	const char* end = text.data() + text.size();
	while (const auto codepoint = Utf8::readCodePoint(currentInText, end)) {
		const auto indexInText = currentInText - text.data();
		currentInText = codepoint->next;
		if (auto charInfo = characterRenderInfo(codepoint->codePoint); charInfo.has_value()) {
			charInfo->indexInText = i32(indexInText);
			return charInfo;
		}
	}

	return std::nullopt;
}