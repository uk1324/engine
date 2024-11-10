add_library(gfx2d "Camera.cpp" "Gfx2d.cpp" "Quad2dPt.cpp" "CameraUtils.cpp" "DbgGfx2d.cpp" "FontRendering/Font.cpp" "FontRendering/FontRenderer.cpp")

include("../codeGenTool/targetAddGenerated.cmake")
targetAddGenerated(gfx2d ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gfx2d PUBLIC gfx)
