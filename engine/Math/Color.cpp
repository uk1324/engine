#include "Color.hpp"
#include <math.h>
#include <engine/Math/Interpolation.hpp>

Vec3 Color3::scientificColoring(float v, float minV, float maxV) {
	v = std::min(std::max(v, minV), maxV - 0.01f);
	auto d = maxV - minV;
	v = d == 0.0f ? 0.5f : (v - minV) / d;
	auto m = 0.25f;
	int num = static_cast<int>(std::floor(v / m));
	auto s = (v - num * m) / m;

	float r = 0.0f, g = 0.0f, b = 0.0f;

	switch (num) {
	case 0: r = 0.0f; g = s; b = 1.0f; break;
	case 1: r = 0.0f; g = 1.0f; b = 1.0f - s; break;
	case 2: r = s; g = 1.0f; b = 0.0f; break;
	case 3: r = 1.0f; g = 1.0f - s; b = 0.0f; break;
	}

	if (r == 0.0f && g == 0.0f && b == 0.0f) {
		g = 1.0f;
	}

	return Vec3(r, g, b);
}

Vec3 Color3::spectral(f32 v) {
	// https://github.com/matplotlib/matplotlib/blob/main/lib/matplotlib/_cm.py
		// _Spectral_data 
	const Vec3 colors[] = {
		Vec3(0.61960784313725492f, 0.003921568627450980f, 0.25882352941176473f),
		Vec3(0.83529411764705885f, 0.24313725490196078f, 0.30980392156862746f),
		Vec3(0.95686274509803926f, 0.42745098039215684f, 0.2627450980392157f),
		Vec3(0.99215686274509807f, 0.68235294117647061f, 0.38039215686274508f),
		Vec3(0.99607843137254903f, 0.8784313725490196f, 0.54509803921568623f),
		Vec3(1.0f, 1.0f, 0.74901960784313726f),
		Vec3(0.90196078431372551f, 0.96078431372549022f, 0.59607843137254901f),
		Vec3(0.6705882352941176f, 0.8666666666666667f, 0.64313725490196083f),
		Vec3(0.4f, 0.76078431372549016f, 0.6470588235294118f),
		Vec3(0.19607843137254902f, 0.53333333333333333f, 0.74117647058823533f),
		Vec3(0.36862745098039218f, 0.30980392156862746f, 0.63529411764705879f)
	};
	const auto colorCount = std::size(colors);
	const auto indexFloat = v * colorCount;
	const auto index = i32(std::floor(indexFloat));
	if (index <= 0) {
		return colors[0];
	}
	if (index >= colorCount - 1) {
		return colors[colorCount - 1];
	}
	const auto t = indexFloat - index;
	return lerp(colors[index], colors[index + 1], t);
}

Vec3 Color3::fromHsv(float h, float s, float v) {
	h = fmodf(h, 1.0f);
	float hue = h * 360.f;

	float C = s * v;
	float X = C * (1.0f - std::abs(fmodf(hue / 60.0f, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (hue >= 0 && hue < 60)
		r = C, g = X, b = 0;
	else if (hue >= 60 && hue < 120)
		r = X, g = C, b = 0;
	else if (hue >= 120 && hue < 180)
		r = 0, g = C, b = X;
	else if (hue >= 180 && hue < 240)
		r = 0, g = X, b = C;
	else if (hue >= 240 && hue < 300)
		r = X, g = 0, b = C;
	else
		r = C, g = 0, b = X;
	return Vec3(r + m, g + m, b + m);
}

Vec3 Color3::toGrayscale(const Vec3& c) {
	auto v = c.x * 0.3f + c.y * 0.59f + c.z * 0.11f;
	return Vec3(v, v, v);
}
