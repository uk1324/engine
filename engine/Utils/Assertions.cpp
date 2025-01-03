#include "Assertions.hpp"
#include "CrashReport.hpp"
#include "DebugBreak.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool assertImplementation(bool condition, const char* functionName, int line) {
	if (condition) {
		return !condition;
	}
	#ifndef FINAL_RELEASE
	DEBUG_BREAK();
	#endif

	#ifdef CLIENT
	crashReportMessageBox("assertion failed", functionName, line);
	#else
	printf(
		"assertion failed\n"
		"functionName = %s\n"
		"line = %d\n",
		functionName, line
	);
	exit(EXIT_FAILURE);
	#endif

	return !condition;
}

void assertNotReachedImplementation(const char* functionName, int line) {
	assertImplementation(false, functionName, line);
}
