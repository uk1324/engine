#pragma once

#include <Types.hpp>
#include <engine/Window.hpp>

namespace Engine {
	void initAll(const Window::Settings& windowSettings, const char* imGuiFontPath);
	void terminateAll();

	void initGlfw();
	void terminateGlfw();

	void initOpenGl();

	void initImGui(const char* fontPath);
	void terminateImGui();

	void updateFrameStart();
	void updateFrameEnd();

	// Must be set before initializing.
	extern int openGlVersionMajor;
	extern int openGlVersionMinor;
};