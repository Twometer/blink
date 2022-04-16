//
// Created by Twometer on 16 Apr 2022.
//

#ifndef BLINK_WINDOW_HPP
#define BLINK_WINDOW_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <map>

class window {
private:
    GLFWwindow *m_handle;

    std::map<int, GLFWcursor *> m_cursors;

public:
    explicit window(GLFWwindow *handle);

    [[nodiscard]] float get_scale() const;

    [[nodiscard]] glm::ivec2 get_size() const;

    void set_cursor(int cursor);

    [[nodiscard]] bool is_key_down(int key) const;

    [[nodiscard]] bool is_mouse_button_down(int btn) const;

    void set_clipboard_content(const std::string &content) const;

    [[nodiscard]] std::string get_clipboard_content() const;

    [[nodiscard]] glm::vec2 get_mouse_pos() const;

};


#endif //BLINK_WINDOW_HPP
