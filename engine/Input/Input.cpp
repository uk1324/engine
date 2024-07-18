#include <engine/Window.hpp>
#include <engine/Input/Input.hpp>
#include <imgui/imgui.h>

auto Input::isKeyDown(KeyCode key) -> bool {
	if (ignoreKeyboardInputs())
		return false;
	const auto code = static_cast<size_t>(key);
	if (code >= keyDown.size()) {
		return false;
	}
	return keyDown[code];
}

auto Input::isKeyDownWithAutoRepeat(KeyCode key) -> bool {
	if (ignoreKeyboardInputs())
		return false;
	const auto code = static_cast<usize>(key);
	if (code >= keyDownWithAutoRepeat.size()) {
		return false;
	}
	return keyDownWithAutoRepeat[code];
}

auto Input::isKeyUp(KeyCode key) -> bool {
	if (ignoreKeyboardInputs())
		return false;
	const auto code = static_cast<usize>(key);
	if (code >= keyUp.size()) {
		return false;
	}
	return keyUp[code];
}

auto Input::isKeyHeld(KeyCode key) -> bool {
	if (ignoreKeyboardInputs())
		return false;
	const auto code = static_cast<usize>(key);
	if (code >= keyHeld.size()) {
		return false;
	}
	return keyHeld[code];
}

bool Input::ignoreKeyboardInputs() {
	return !ignoreImGuiWantCapture && ImGui::GetIO().WantCaptureKeyboard;
}

auto Input::isMouseButtonDown(MouseButton button) -> bool {
	if (!ignoreImGuiWantCapture && ImGui::GetIO().WantCaptureMouse)
		return false;
	return isKeyDown(static_cast<KeyCode>(button));
}

auto Input::isMouseButtonUp(MouseButton button) -> bool {
	if (!ignoreImGuiWantCapture && ImGui::GetIO().WantCaptureMouse)
		return false;
	return isKeyUp(static_cast<KeyCode>(button));
}

auto Input::isMouseButtonHeld(MouseButton button) -> bool {
	if (!ignoreImGuiWantCapture && ImGui::GetIO().WantCaptureMouse)
		return false;
	return isKeyHeld(static_cast<KeyCode>(button));
}

auto Input::windowSpaceToClipSpace(Vec2 v) -> Vec2 {
	v /= (Window::size() / 2.0f);
	v.y = -v.y;
	v += Vec2{ -1.0f, 1.0f };
	return v;
}

auto Input::scrollDelta() -> float {
	if (!ignoreImGuiWantCapture && ImGui::GetIO().WantCaptureMouse)
		return 0.0f;
	return scrollDelta_;
}

auto Input::anyKeyPressed() -> bool {
	return anyKeyPressed_;
}

std::optional<KeyCode> Input::lastKeycodeDownThisFrame() {
	return lastKeycodeDownThisFrame_;
}

auto Input::update() -> void {
	keyDown.reset();
	keyDownWithAutoRepeat.reset();
	keyUp.reset();

	scrollDelta_ = 0.0f;
	anyKeyPressed_ = false;
	lastKeycodeDownThisFrame_ = std::nullopt;
}

//static auto setIfAlreadyExists(std::unordered_map<int, bool>& map, int key, bool value) -> void {
//	if (auto it = map.find(key); it != map.end())
//		it->second = value;
//}

static auto isMouseButton(u16 vkCode) -> bool {
	const auto code = static_cast<MouseButton>(vkCode);
	return code == MouseButton::LEFT || code == MouseButton::RIGHT || code == MouseButton::MIDDLE;
}

static auto isKeyboardKey(u16 vkCode) -> bool {
	return !isMouseButton(vkCode);
}

auto Input::onKeyDown(u16 virtualKeyCode, bool autoRepeat) -> void {
	if (virtualKeyCode >= VIRTUAL_KEY_COUNT)
		return;

	anyKeyPressed_ = true;

	if (!autoRepeat) {
		keyDown.set(virtualKeyCode);
		keyHeld.set(virtualKeyCode);
	}
	keyDownWithAutoRepeat.set(virtualKeyCode);

	if (!isMouseButton(virtualKeyCode)) {
		lastKeycodeDownThisFrame_ = static_cast<KeyCode>(virtualKeyCode);
	}
}

auto Input::onKeyUp(u16 virtualKeyCode) -> void {
	if (virtualKeyCode >= VIRTUAL_KEY_COUNT)
		return;

	keyUp.set(virtualKeyCode);
	keyHeld.set(virtualKeyCode, false);
}

auto Input::onMouseMove(Vec2 mousePos) -> void {
	cursorPosClipSpace_ = windowSpaceToClipSpace(mousePos);
	cursorPosWindowSpace_ = mousePos;
}

auto Input::onMouseScroll(float scroll) -> void {
	scrollDelta_ = scroll;
}

bool Input::ignoreImGuiWantCapture = false;

std::bitset<Input::VIRTUAL_KEY_COUNT> Input::keyDown;
std::bitset<Input::VIRTUAL_KEY_COUNT> Input::keyUp;
std::bitset<Input::VIRTUAL_KEY_COUNT> Input::keyDownWithAutoRepeat;
std::bitset<Input::VIRTUAL_KEY_COUNT> Input::keyHeld;

Vec2 Input::cursorPosClipSpace_ = Vec2(0.0f);
Vec2 Input::cursorPosWindowSpace_ = Vec2(0.0f);
float Input::scrollDelta_ = 0.0f;
bool Input::anyKeyPressed_ = false;
std::optional<KeyCode> Input::lastKeycodeDownThisFrame_ = std::nullopt;