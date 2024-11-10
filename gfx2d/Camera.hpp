#pragma once

#include <engine/Math/Mat3x2.hpp>
#include <engine/Math/Aabb.hpp>

Mat3x2 makeObjectTransform(Vec2 pos, float rotation, Vec2 scale);

// When zoom is 1 the width goes from -1 to 1. So the width is 2.
struct Camera {
	Camera(Vec2 pos = Vec2{ 0.0f }, float zoom = 1.0f);

	Mat3x2 toNdc() const;
	Mat3x2 cameraTransform() const;
	Mat3x2 clipSpaceToWorldSpace() const;
	Mat3x2 makeTransform(Vec2 pos, float rotation, Vec2 scale) const;
	Mat3x2 worldToCameraToNdc() const;
	float height() const;
	float width() const;
	void setHeight(float height);
	void setWidth(float width);
	void changeSizeToFitBox(const Vec2& boxSize);

	Aabb aabb() const;

	Vec2 pos;
	float zoom;
	// Aspect ratio should be width / height = x / y.
	float aspectRatio;
};