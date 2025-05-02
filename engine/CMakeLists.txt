project(engine)

add_library(engine STATIC 
	"../dependencies/imgui/imgui.cpp" "../dependencies/imgui/imgui_draw.cpp" "../dependencies/imgui/imgui_impl_glfw.cpp" "../dependencies/imgui/imgui_tables.cpp" "../dependencies/imgui/imgui_widgets.cpp" "../dependencies/imgui/imgui_impl_opengl3.cpp" "../dependencies/imgui/imgui_impl_opengl3.h" "../dependencies/imgui/imgui_demo.cpp" "../dependencies/imgui/implot.cpp" "../dependencies/imgui/implot_demo.cpp" "../dependencies/imgui/implot_items.cpp" "../dependencies/imgui/imgui_stdlib.cpp"

	"../dependencies/stb_image/stb_image.cpp" "../dependencies/stb_image/stb_image_resize.cpp" "../dependencies/stb_image/stb_image_write.cpp"

	"../dependencies/stb_vorbis/stb_vorbis.cpp"

	"../dependencies/glad/src/glad.c"

	"Math/Aabb.cpp"

	"Engine.cpp" "EngineUpdateLoop.cpp" "Window.cpp" "Input/Input.cpp" "Input/InputUtils.cpp" "Utils/Timer.cpp" "Utils/Assertions.cpp" "Utils/CrashReport.cpp" "Utils/Put.cpp" "Utils/Gui.cpp" "Utils/FixedUpdateLoop.cpp" "Utils/StringStream.cpp" "Utils/PreprocessIncludes.cpp" "Graphics/Vao.cpp" "Graphics/Vbo.cpp" "Graphics/ShaderProgram.cpp" "Graphics/Shader.cpp" "Graphics/Ibo.cpp" "Graphics/Texture.cpp" "Graphics/Fbo.cpp" "Utils/Image32.cpp" "Utils/FileIo.cpp" "Math/Color.cpp" "Math/Rotation.cpp" "Utils/View.natvis" "Utils/List.natvis" "Math/LineSegment.cpp" "Math/Line.cpp" "Math/ComplexPolygonOutline.cpp" "Math/Triangulate.cpp" "Math/SimplePolygon.cpp" "Math/Triangle.cpp" "Math/DouglassPecker.cpp"  "Math/PointInShape.cpp" "Math/ShapeAabb.cpp" "../dependencies/Json/JsonValue.cpp" "../dependencies/Json/JsonParser.cpp" "../dependencies/Json/JsonPrinter.cpp""Utils/HashCombine.cpp" "Utils/Json.cpp" "Utils/JsonFileIo.cpp" "Math/MarchingSquares.cpp" "Utils/Utf8.cpp" "Math/Interpolation.cpp" "Audio/Audio.cpp" "Audio/AudioBuffer.cpp" "Audio/AudioFileStream.cpp"  "Audio/AudioErrorHandling.cpp" "Audio/AudioSource.cpp" "Math/Angles.cpp" "Math/Plane.cpp" "Math/Ray3.cpp" "Math/Sphere.cpp" "Math/Circle.cpp" "Math/GramSchmidt.cpp")


target_include_directories(engine PUBLIC Utils "../")


target_include_directories(engine PUBLIC "../dependencies/glad/include")
target_include_directories(engine PUBLIC "../dependencies")

target_link_libraries(engine PRIVATE glfw)
target_include_directories(engine PRIVATE "../dependencies/glfw/include")

target_link_libraries(engine PRIVATE OpenAL)
target_include_directories(engine PUBLIC "../dependencies/openal-soft/include")

target_compile_features(engine PUBLIC cxx_std_23)
set_target_properties(engine PROPERTIES CXX_EXTENSIONS OFF)