#include <engine/Engine.hpp>
#include <engine/Log.hpp>
#include <engine/Input/Input.hpp>
#include <Timer.hpp>
#include <Put.hpp>
#include <opengl/gl.h>
#include <GLFW/glfw3.h>
#include <imgui/implot.h>
#ifdef __EMSCRIPTEN__
#include <GLFW/emscripten_glfw3.h>
#endif

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

static void glfwErrorCallback(int errorCode, const char* errorMessage) {
	//Log::fatal("glfw error % %", errorCode, errorMessage);
	Log::error("glfw error % %", errorCode, errorMessage);
}

#ifndef __EMSCRIPTEN__
static void openGlErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	// Fragment spriteShader recompiled due to state change.
	if (id == 2)
		return;

	// glLineWidth deprecated.
	if (id == 7)
		return;

	// Read pixels
	// Performance
	// Pixel-path performance warning: Pixel transfer is synchronized with 3D rendering.
	if (id == 131154)
		return;

	std::string errorMessage = "source: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             errorMessage += "api"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   errorMessage += "window system"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: errorMessage += "shader compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     errorMessage += "third party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     errorMessage += "application"; break;
	case GL_DEBUG_SOURCE_OTHER:           errorMessage += "other"; break;
	}
	errorMessage += '\n';

	errorMessage += " type: ";
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               errorMessage += "error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: errorMessage += "deprecated behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  errorMessage += "undefined behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         errorMessage += "portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         errorMessage += "performance"; break;
	case GL_DEBUG_TYPE_MARKER:              errorMessage += "marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          errorMessage += "push group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           errorMessage += "pop group"; break;
	case GL_DEBUG_TYPE_OTHER:               errorMessage += "other"; break;
	}
	errorMessage += '\n';

	errorMessage += " severity: ";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         errorMessage += "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       errorMessage += "medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          errorMessage += "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: errorMessage += "notification"; break;
	}
	errorMessage += '\n';

	errorMessage += " message: ";
	errorMessage += message;
	errorMessage += '\n';

	if (id == 1286 || severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
		Log::fatal("OpenGL error %", errorMessage);
	} else {
		Log::fatal("OpenGL error %", errorMessage);
	}
}
#endif

void Engine::initAll(const Window::Settings& windowSettings, const char* imGuiFontPath, const char* imGuiIniPath) {
	Timer timer;
	#ifdef DEBUG
	put("Engine::init start");
	#endif
	initGlfw();
	{
		Timer timer;
		Window::init(windowSettings);
		#ifdef DEBUG
		put("Window::init took: %", timer.elapsedMilliseconds());
		#endif
	}
	initOpenGl();
	initImGui(imGuiFontPath, imGuiIniPath);

	#ifdef DEBUG
	put("Engine::init took: %", timer.elapsedMilliseconds());
	#endif
}

void Engine::terminateAll() {
	terminateImGui();
	Window::terminate();
	terminateGlfw();
}

void Engine::initGlfw() {
	#ifdef __EMSCRIPTEN__
	glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_EMSCRIPTEN);
	#endif 

	if (glfwInit() == GLFW_FALSE) {
		Log::fatal("failed to initialize GLFW");
	}
	glfwSetErrorCallback(glfwErrorCallback);
}

void Engine::terminateGlfw() {
	glfwTerminate();
}

void Engine::initOpenGl() {
	#ifndef __EMSCRIPTEN__
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false) {
		Log::fatal("failed to initialize OpenGL");
	}

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openGlErrorCallback, nullptr);
	} else {
		Log::error("failed to initialize debug output");
	}
	#endif
}

void Engine::initImGui(const char* fontPath, const char* imGuiIniPath) {
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	if (fontPath != nullptr) {
		io.Fonts->AddFontFromFileTTF(fontPath, 20.0f);
	}
	
	/*io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;*/
	auto& style = ImGui::GetStyle();
	style.WindowRounding = 5.0f;

	if (imGuiIniPath != nullptr) {
		ImGui::GetIO().IniFilename = imGuiIniPath;
	}

	ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(Window::handle()), true);
	#ifdef __EMSCRIPTEN__
	ImGui_ImplOpenGL3_Init("#version 300 es");
	#else
	ImGui_ImplOpenGL3_Init("#version 430");
	#endif
}

void Engine::terminateImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void Engine::updateFrameStart() {
	Input::update();
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Engine::updateFrameEnd() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	Window::update();
}

int Engine::openGlVersionMajor = 4;
int Engine::openGlVersionMinor = 3;