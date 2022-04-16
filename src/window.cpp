//
// Created by Twometer on 16 Apr 2022.
//

#include "window.hpp"

window::window(GLFWwindow *handle) : m_handle(handle) {
}

float window::get_scale() const {
    float scale;
    glfwGetWindowContentScale(m_handle, &scale, &scale);
    return scale;
}

glm::ivec2 window::get_size() const {
    int w, h;
    glfwGetFramebufferSize(m_handle, &w, &h);
    return {w, h};
}

void window::set_cursor(int cursor) {
    if (!m_cursors.contains(cursor)) {
        m_cursors[cursor] = glfwCreateStandardCursor(cursor);
    }
    glfwSetCursor(m_handle, m_cursors[cursor]);
}

bool window::is_key_down(int key) const {
    return glfwGetKey(m_handle, key) == GLFW_PRESS;
}

bool window::is_mouse_button_down(int btn) const {
    return glfwGetMouseButton(m_handle, btn) == GLFW_PRESS;
}

void window::set_clipboard_content(const std::string &content) const {
    glfwSetClipboardString(m_handle, content.c_str());
}

std::string window::get_clipboard_content() const {
    return glfwGetClipboardString(m_handle);
}

glm::vec2 window::get_mouse_pos() const {
    double x, y;
    glfwGetCursorPos(m_handle, &x, &y);
    return {x, y};
}
