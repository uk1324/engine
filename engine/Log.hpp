#pragma once

#include <Put.hpp>
#include <DebugBreak.hpp>
#include <CrashReport.hpp>
#include <FixedSizeStringStream.hpp>

namespace Log {
	template<typename ...Args>
	void error(const char* format, const Args&... args);

	template<typename ...Args>
	[[noreturn]] void fatal(const char* format, const Args&... args);

	// Could contatenate strings in macro.

	template<typename ...Args>
	void warning(const char* format, const Args& ...args) {
		put(std::cerr, "[ERROR] ");
		put(std::cerr, format, args...);
	}

	template<typename ...Args>
	void error(const char* format, const Args& ...args) {
		put(std::cerr, "[ERROR] ");
		put(std::cerr, format, args...);
	}

	template<typename ...Args>
	[[noreturn]] void fatal(const char* format, const Args& ...args) {
	#ifdef FINAL_RELEASE
		FixedSizeStringStream<1024> s;
		put(s, format, args...);
		crashReportMessageBox("%s", s.str());
	#else
		DEBUG_BREAK();
		putnn(std::cerr, "[FATAL] ");
		put(std::cerr, format, args...);
	#endif
		exit(EXIT_FAILURE);
	}
}