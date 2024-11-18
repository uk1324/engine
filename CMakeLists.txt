cmake_minimum_required(VERSION 3.15)

add_subdirectory("./dependencies/glfw")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("./dependencies/openal-soft")

add_subdirectory(engine)
add_subdirectory(gfx)
add_subdirectory(gfx2d)