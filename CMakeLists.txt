cmake_minimum_required(VERSION 3.19)
project(blink)

set(CMAKE_CXX_STANDARD 20)

set(GLAD_SOURCES
        lib/glad/src/glad.c)

set(HB_SOURCES
        lib/harfbuzz/src/harfbuzz.cc)

set(BLINK_SOURCES
        src/main.cpp)

add_subdirectory(lib/glfw)

add_executable(blink ${GLAD_SOURCES} ${HB_SOURCES} ${BLINK_SOURCES})
#set_target_properties(blink PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

target_include_directories(blink PRIVATE lib/glad/include lib/glm lib/glfw/include lib/harfbuzz/src lib/freetype/include)
target_link_directories(blink PRIVATE lib/freetype)
target_link_libraries(blink glfw ${GLFW_LIBRARIES} freetype)

file(COPY lib/freetype/freetype.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})