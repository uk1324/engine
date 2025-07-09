cmake_minimum_required(VERSION 3.15)

if (NOT EMSCRIPTEN)
	add_subdirectory("./dependencies/glfw")
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

	set(ALSOFT_EXAMPLES OFF)
	set(ALSOFT_UTILS OFF)
	add_subdirectory("./dependencies/openal-soft")
endif()

add_subdirectory(engine)
add_subdirectory(gfx)
add_subdirectory(gfx2d)