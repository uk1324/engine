#include <engine/Window.hpp>
#include <engine/Engine.hpp>
#include <engine/Input/Input.hpp>
#include <engine/Log.hpp>
#include <GLFW/glfw3.h>
#ifdef __EMSCRIPTEN__
#include <GLFW/emscripten_glfw3.h>
#include <emscripten/html5.h>
#endif

static GLFWwindow* windowHandle = nullptr;
static bool resizedThisFrame = true;

static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		Input::onKeyDown(key, false);
	} else if (action == GLFW_RELEASE) {
		Input::onKeyUp(key);
	} else if (action == GLFW_REPEAT) {
		Input::onKeyDown(key, true);
	}
}

static void mouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY) {
	Input::onMouseMove(Vec2(static_cast<float>(mouseX), static_cast<float>(mouseY)));
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	// @Hack: Assumes the button ids don't conflinct with keyboard ids.
	if (action == GLFW_PRESS) {
		Input::onKeyDown(button, false);
	} else if (action == GLFW_RELEASE) {
		Input::onKeyUp(button);
	}
}

static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	Input::onMouseScroll(static_cast<float>(yOffset));
}

static void windowResizeCallback(GLFWwindow* window, int width, int height) {
	resizedThisFrame = true;
}

void Window::init(const Settings& settings) {
	if (settings.maximized) {
		#ifndef __EMSCRIPTEN__
		glfwWindowHint(GLFW_MAXIMIZED, true);
		#endif
	}

	if (settings.openGlDebugContext) {
		#ifndef __EMSCRIPTEN__
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif
	}
	glfwWindowHint(GLFW_SAMPLES, settings.multisamplingSamplesPerPixel);
	#ifdef __EMSCRIPTEN__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GLFW_TRUE);
	#else
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Engine::openGlVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Engine::openGlVersionMinor);
	#endif 


	
	//const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	///*window_width = mode->width;
	//window_height = mode->height;*/

	#ifdef __EMSCRIPTEN__
	emscripten_glfw_set_next_window_canvas_selector("#canvas");
	#endif

	windowHandle = glfwCreateWindow(settings.width, settings.height, settings.title, nullptr, nullptr);
	//windowHandle = glfwCreateWindow(mode->width, mode->height, settings.title, glfwGetPrimaryMonitor(), nullptr);
	if (windowHandle == nullptr) {
		Log::fatal("failed to create window");
	}
	glfwMakeContextCurrent(windowHandle);

	glfwSetKeyCallback(windowHandle, keyboardCallback);
	glfwSetCursorPosCallback(windowHandle, mouseMoveCallback);
	glfwSetMouseButtonCallback(windowHandle, mouseButtonCallback);
	glfwSetScrollCallback(windowHandle, mouseScrollCallback);
	glfwSetWindowSizeCallback(windowHandle, windowResizeCallback);
	#ifdef __EMSCRIPTEN__
	emscripten_set_mousemove_callback(
		"#canvas", nullptr, false,
		+[](int eventType, const EmscriptenMouseEvent* mouseEvent, void* userData) -> bool {
			
			Input::onMouseMoveEvent(mouseEvent->movementX, mouseEvent->movementY);
			return true;
		}
	);
	emscripten_glfw_make_canvas_resizable(windowHandle, "window", nullptr);
	#endif 


	{
		// Prevent cursor from jumping on first mouse move because before that the mouse position is uninitialized.
		double x, y;
		glfwGetCursorPos(windowHandle, &x, &y);
		Input::onMouseMove(Vec2(static_cast<float>(x), static_cast<float>(y)));
	}
}

void Window::terminate() {
	glfwDestroyWindow(windowHandle);
}

Vec2 Window::size() {
	int x, y;
	glfwGetFramebufferSize(windowHandle, &x, &y);
	return Vec2{ static_cast<float>(x), static_cast<float>(y) };
}

float Window::aspectRatio() {
	const auto size = Window::size();
	return size.x / size.y;
}

const char* Window::getClipboard() {
	return glfwGetClipboardString(windowHandle);
}

void Window::setClipboard(const char* string) {
	glfwSetClipboardString(windowHandle, string);
}

void Window::update() {
	resizedThisFrame = false;
	#ifndef __EMSCRIPTEN__
	glfwSwapBuffers(windowHandle);
	#endif
}

void Window::close() {
	glfwSetWindowShouldClose(windowHandle, true);
}

void Window::setTitle(const char* title) {
	glfwSetWindowTitle(windowHandle, title);
}

void Window::setPos(Vec2 pos) {
	glfwSetWindowPos(windowHandle, static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void Window::setSize(Vec2T<int> size) {
	glfwSetWindowSize(windowHandle, size.x, size.y);
}

void Window::minimize() {
	glfwRestoreWindow(windowHandle);
}
void Window::enableWindowedFullscreen() {
	//    if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
	//    {
	//        glfwRestoreWindow(engine.window().handle());
	//    }
	//    else
	//    {
	//        glfwMaximizeWindow(engine.window().handle());
	//    }dsfsdfdsffvfdgwdwdwd
	glfwMaximizeWindow(windowHandle);
}

struct BeforeFullscreen {
	int posX;
	int posY;
	int sizeX;
	int sizeY;
};

static std::optional<BeforeFullscreen> beforeFullscreen;

void Window::setFullscreen(bool fullscreen) {
	auto isFullscreenEnabled = []() {
		return glfwGetWindowMonitor(windowHandle) != nullptr;
	};

	if (isFullscreenEnabled() == fullscreen) {
		return;
	}

	if (fullscreen) {
		BeforeFullscreen b;
		glfwGetWindowPos(windowHandle, &b.posX, &b.posY);
		glfwGetWindowSize(windowHandle, &b.sizeX, &b.sizeY);
		beforeFullscreen = b;
		const auto monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(windowHandle, monitor, 0, 0, mode->width, mode->height, 0);
	} else {
		if (beforeFullscreen.has_value()) {
			glfwSetWindowMonitor(windowHandle, nullptr, beforeFullscreen->posX, beforeFullscreen->posY, beforeFullscreen->sizeX, beforeFullscreen->sizeY, 0);
		} else {
			// Just to be safe. Normally the window is lauched the set to fullscren. So you can just set the size before setting to fullscreen.
			glfwSetWindowMonitor(windowHandle, nullptr, 0, 0, 600, 480, 0);
		}
	}
}

void Window::disableCursor() {
	glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::enableCursor() {
	glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::toggleCursor() {
	const auto mode = glfwGetInputMode(windowHandle, GLFW_CURSOR);
	if (mode == GLFW_CURSOR_NORMAL) {
		disableCursor();
	} else if (mode == GLFW_CURSOR_DISABLED) {
		enableCursor();
	} else {
		// GLFW_CURSOR_HIDDEN what to do?
		ASSERT_NOT_REACHED();
	}
}

bool Window::isCursorEnabled() {
	const auto mode = glfwGetInputMode(windowHandle, GLFW_CURSOR);
	return mode == GLFW_CURSOR_NORMAL;
}

bool Window::resized() {
	return resizedThisFrame;
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(windowHandle);
}

void* Window::handle() {
	return windowHandle;
}