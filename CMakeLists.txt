cmake_minimum_required(VERSION 3.19)
project(blink)

set(CMAKE_CXX_STANDARD 20)

set(GLAD_SOURCES
        lib/glad/src/glad.c)

set(BLINK_SOURCES
        src/main.cpp)

add_subdirectory(lib/glfw)

add_executable(blink ${GLAD_SOURCES} ${BLINK_SOURCES})

target_include_directories(blink PUBLIC lib/glad/include lib/glm lib/glfw/include)

target_link_libraries(blink glfw ${GLFW_LIBRARIES})

#set_target_properties(blink PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")