#include "CrashReport.hpp"
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#elif linux

#endif

[[noreturn]] void crashReportMessageBoxWithLocation(const char* text, const char* functionName, int line) {
	crashReportMessageBox("line = %d\nfunction = %s\nmessage\n%s", line, functionName, text);
}

[[noreturn]] void crashReportMessageBox(const char* text, ...) {
	va_list args;
	va_start(args, text);
	char message[1024];
	vsnprintf(message, sizeof(message), text, args);
	#ifdef _WIN32
	MessageBoxA(nullptr, message, "error", MB_OK | MB_ICONEXCLAMATION);
	#endif // For other platforms maybe use GTK or SDL. 
	va_end(args);
	exit(EXIT_FAILURE);
}
