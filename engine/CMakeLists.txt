project(engine)

add_library(engine STATIC 
	"../dependencies/imgui/imgui.cpp" "../dependencies/imgui/imgui_draw.cpp" "../dependencies/imgui/imgui_impl_glfw.cpp" "../dependencies/imgui/imgui_tables.cpp" "../dependencies/imgui/imgui_widgets.cpp" "../dependencies/imgui/imgui_impl_opengl3.cpp" "../dependencies/imgui/imgui_impl_opengl3.h" "../dependencies/imgui/imgui_demo.cpp" "../dependencies/imgui/implot.cpp" "../dependencies/imgui/implot_demo.cpp" "../dependencies/imgui/implot_items.cpp" "../dependencies/imgui/imgui_stdlib.cpp"

	"../dependencies/stb_image/stb_image.cpp" "../dependencies/stb_image/stb_image_resize.cpp" "../dependencies/stb_image/stb_image_write.cpp"

	"../dependencies/glad/src/glad.c"

	"Math/Aabb.cpp"

	"Engine.cpp" "EngineUpdateLoop.cpp" "Window.cpp" "Input/Input.cpp" "Utils/Timer.cpp" "Utils/Assertions.cpp" "Utils/CrashReport.cpp" "Utils/Put.cpp" "Utils/Gui.cpp" "Utils/FixedUpdateLoop.cpp" "Utils/StringStream.cpp" "Utils/PreprocessIncludes.cpp" "Graphics/Vao.cpp" "Graphics/Vbo.cpp" "Graphics/ShaderProgram.cpp" "Graphics/Shader.cpp" "Graphics/Ibo.cpp" "Graphics/Texture.cpp" "Utils/Image32.cpp" "Utils/FileIo.cpp")

target_link_libraries(engine PRIVATE glfw)

target_include_directories(engine PUBLIC Utils "../")


target_include_directories(engine PUBLIC "../dependencies/glad/include")
target_include_directories(engine PUBLIC "../dependencies")

target_include_directories(engine PRIVATE "../dependencies/glfw/include")

target_compile_features(engine PUBLIC cxx_std_23)
set_target_properties(engine PROPERTIES CXX_EXTENSIONS OFF)