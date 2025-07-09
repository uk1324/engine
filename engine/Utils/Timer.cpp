#include "Timer.hpp"
#include "Put.hpp"
#include "Gui.hpp"

Timer::Timer()
	: start(std::chrono::high_resolution_clock::now()) {}

void Timer::reset() {
	start = std::chrono::high_resolution_clock::now();
}

float Timer::elapsedMilliseconds() const {
	std::chrono::duration<float, std::milli> elapsed = (std::chrono::high_resolution_clock::now() - start);
	return elapsed.count();
}

float Timer::elapsedSeconds() const {
	return elapsedMilliseconds() / 1000.0f;
}

void Timer::tookSeconds(std::string_view whatTook) const {
	//#ifdef DEBUG
	put("% took % s", whatTook, elapsedSeconds());
	//#endif

}

void Timer::tookMs(std::string_view whatTook) const {
	//#ifdef DEBUG
	put("% took % ms", whatTook, elapsedMilliseconds());
	//#endif

}

void Timer::guiTookMiliseconds(std::string whatTook) const {
	Gui::put("% took % ms", whatTook, elapsedMilliseconds());
}
