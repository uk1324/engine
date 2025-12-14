#pragma once

[[noreturn]] void crashReportMessageBoxWithLocation(const char* text, const char* functionName, int line);
[[noreturn]] void crashReportMessageBox(const char* format, ...);