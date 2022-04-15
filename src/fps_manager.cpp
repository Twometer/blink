//
// Created by Tim Buchner on 15.04.22.
//

#include "fps_manager.hpp"
#include <GLFW/glfw3.h>

void fps_manager::on_frame_begin() {
    m_frame_start = glfwGetTime();
}

void fps_manager::on_frame_end() {
    double now = glfwGetTime();
    m_frame_counter++;
    m_frame_time = now - m_frame_start;
    if (now - m_last_frame > 1) {
        m_fps = m_frame_counter;
        m_frame_counter = 0;
        m_last_frame = now;
    }
}

void fps_manager::limit(int fps) const {
    double expected_time = 1.0 / fps;
    double remaining = expected_time - m_frame_time;
    if (remaining > 0) {

    }
}

