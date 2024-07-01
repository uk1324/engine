#pragma once

#include <engine/Math/Vec2.hpp>
#include <engine/Input/KeyCode.hpp>
#include <engine/Input/MouseButton.hpp>
#include <Types.hpp>

#include <bitset>
#include <optional>

// TODO: Could make a sum type that would store any type of button.

// There is no way to make private variables without a class if templated functions are used and also the on<action> functions couldn't be private.
class Input {
public:
	static auto isKeyDown(KeyCode key) -> bool;
	static auto isKeyDownWithAutoRepeat(KeyCode key) -> bool;
	static auto isKeyUp(KeyCode key) -> bool;
	static auto isKeyHeld(KeyCode key) -> bool;

	static bool ignoreKeyboardInputs();

	static auto isMouseButtonDown(MouseButton button) -> bool;
	static auto isMouseButtonUp(MouseButton button) -> bool;
	static auto isMouseButtonHeld(MouseButton button) -> bool;

	static auto windowSpaceToClipSpace(Vec2 v) -> Vec2;
	static auto cursorPosClipSpace() -> Vec2 { return cursorPosClipSpace_; };
	static auto cursorPosWindowSpace() -> Vec2 { return cursorPosWindowSpace_; };
	// Number of times scrolled this frame. If normal scrolling then 1 if fast then more.
	static auto scrollDelta() -> float;
	static auto anyKeyPressed() -> bool;
	static std::optional<KeyCode> lastKeycodeDownThisFrame();

	static auto update() -> void;

	static bool ignoreImGuiWantCapture;

	static auto onKeyDown(u16 virtualKeyCode, bool autoRepeat) -> void;
	static auto onKeyUp(u16 virtualKeyCode) -> void;
	static auto onMouseMove(Vec2 mousePos) -> void;
	static auto onMouseScroll(float scroll) -> void;
private:

	static constexpr auto MOUSE_BUTTON_COUNT = static_cast<size_t>(MouseButton::COUNT);
	static constexpr auto KEYCODE_COUNT = static_cast<size_t>(KeyCode::COUNT);
	// Virtual keycodes are always one byte so this is kind of pointless.
	static constexpr auto VIRTUAL_KEY_COUNT = (MOUSE_BUTTON_COUNT > KEYCODE_COUNT) ? MOUSE_BUTTON_COUNT : KEYCODE_COUNT;
	static std::bitset<VIRTUAL_KEY_COUNT> keyDown;
	static std::bitset<VIRTUAL_KEY_COUNT> keyDownWithAutoRepeat;
	// KeyUp doesn't get auto repeated.
	static std::bitset<VIRTUAL_KEY_COUNT> keyUp;
	static std::bitset<VIRTUAL_KEY_COUNT> keyHeld;

	static Vec2 cursorPosClipSpace_;
	static Vec2 cursorPosWindowSpace_;
	static float scrollDelta_;
	static bool anyKeyPressed_;
	static std::optional<KeyCode> lastKeycodeDownThisFrame_;
};
