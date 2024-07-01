#include "Camera.hpp"
#include <engine/Math/Aabb.hpp>
#include <engine/Window.hpp>
#include <engine/Input/Input.hpp>

Mat3x2 makeObjectTransform(Vec2 pos, float rotation, Vec2 scale) {
	return Mat3x2::scale(scale) * Mat3x2::rotate(rotation) * Mat3x2::translate(Vec2(pos.x, pos.y));
}

Camera::Camera(Vec2 pos, float zoom)
	: pos{ pos }
	, zoom{ zoom }
	, aspectRatio{ Window::aspectRatio() } // This is here to prevent bugs from not having the aspect ratio initialized correctly. If the camera is passed into the renderer this should set correctly after the frist frame, but if it is only created in the local scope it is really easy to forget to initialize it.
{}

Mat3x2 Camera::toNdc() const {
	return Mat3x2::scale(Vec2(1.0f / aspectRatio, 1.0f));
}

Mat3x2 Camera::cameraTransform() const {
	return Mat3x2::translate(-pos) * Mat3x2::scale(Vec2(zoom));
}

Mat3x2 Camera::clipSpaceToWorldSpace() const {
	return (cameraTransform() * toNdc()).inversed();
}

Mat3x2 Camera::makeTransform(Vec2 pos, float rotation, Vec2 scale) const {
	const auto objectToWorld = makeObjectTransform(pos, rotation, scale);
	return objectToWorld * worldToCameraToNdc();
}

Mat3x2 Camera::worldToCameraToNdc() const {
	return cameraTransform() * toNdc();
}

float Camera::height() const {
	return 2.0f / zoom;
}

float Camera::width() const {
	//return 2.0f * aspectRatio / zoom;
	return height() * aspectRatio;
}

//auto Camera::setWidth(float width) -> void {
//	zoom = 2.0f * aspectRatio / width;
//}
//
//auto Camera::setHeight(float height) -> void {
//	setWidth(height * aspectRatio);
//}

void Camera::setHeight(float height) {
	// height = 2.0f / zoom
	// zoom * height = 2.0f 
	// zoom = 2.0f / height 
	zoom = 2.0f / height;
}

/*
tests
//camera.setHeight(20.0f);
//const auto a = camera.height();

//camera.setWidth(20.0f);
//const auto b = camera.width();

//instances.push_back(TexturedQuadInstance{ camera.makeTransform(Vec2(0.0f), 0.0f, Vec2(0.1f))});
instances.push_back(TexturedQuadInstance{ camera.makeTransform(Vec2(camera.width() / 2.0f, 0.0f), 0.0f, Vec2(0.1f)) });
instances.push_back(TexturedQuadInstance{ camera.makeTransform(Vec2(-camera.width() / 2.0f, 0.0f), 0.0f, Vec2(0.1f)) });
instances.push_back(TexturedQuadInstance{ camera.makeTransform(Vec2(0.0f, camera.height() / 2.0f), 0.0f, Vec2(0.1f)) });
instances.push_back(TexturedQuadInstance{ camera.makeTransform(Vec2(0.0f, -camera.height() / 2.0f), 0.0f, Vec2(0.1f)) });
*/

void Camera::setWidth(float width) {
	setHeight(width / aspectRatio);
}

void Camera::changeSizeToFitBox(const Vec2& boxSize) {
	setWidth(boxSize.x);
	if (boxSize.y > height()) {
		setHeight(boxSize.y);
	}
}

Aabb Camera::aabb() const {
	const auto halfSize = Vec2{ width(), height() } / 2.0f;
	return Aabb::fromCorners(pos - halfSize, pos + halfSize);
}
