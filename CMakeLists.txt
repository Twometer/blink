cmake_minimum_required(VERSION 3.19)
project(blink)

set(CMAKE_CXX_STANDARD 20)

set(GLAD_SOURCES
        lib/glad/src/glad.c)

set(HB_SOURCES
        lib/harfbuzz/src/harfbuzz.cc)

set(IMGUI_SOURCES
        lib/imgui/imgui.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp
        lib/imgui/backends/imgui_impl_glfw.cpp)

set(BLINK_SOURCES
        src/main.cpp src/shader_src.hpp src/loader.cpp src/loader.hpp src/shader.cpp src/shader.hpp src/font.cpp src/font.hpp src/freetype.cpp src/freetype.hpp src/atlas.cpp src/atlas.hpp src/main.cpp src/renderer.cpp src/renderer.hpp src/text_buffer.cpp src/text_buffer.hpp src/document.cpp src/document.hpp src/mesh.cpp src/mesh.hpp src/fps_manager.cpp src/fps_manager.hpp src/range.cpp src/range.hpp src/cursor.hpp src/editor.cpp src/editor.hpp src/theme.hpp src/window.cpp src/window.hpp src/render_context.hpp src/imgui_style.hpp)

add_subdirectory(lib/glfw)

add_executable(blink ${GLAD_SOURCES} ${HB_SOURCES} ${IMGUI_SOURCES} ${BLINK_SOURCES})

target_include_directories(blink PRIVATE lib/glad/include lib/glm lib/glfw/include lib/harfbuzz/src lib/freetype/include lib/imgui)
target_link_directories(blink PRIVATE lib/freetype)
target_link_libraries(blink glfw ${GLFW_LIBRARIES} freetype)

if (WIN32)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set_target_properties(blink PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
    endif ()
    file(COPY lib/freetype/freetype.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
elseif (APPLE)
    target_link_directories(blink PRIVATE /opt/homebrew/Cellar/freetype/2.12.0/lib)
endif ()

