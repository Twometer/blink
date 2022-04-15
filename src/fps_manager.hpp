//
// Created by Twometer on 15 Apr 2022.
//

#ifndef BLINK_FPS_MANAGER_HPP
#define BLINK_FPS_MANAGER_HPP


class fps_manager {
private:
    double m_last_frame = 0;
    double m_frame_start = 0;
    double m_frame_time = 0;
    int m_frame_counter = 0;
    int m_fps = 0;

public:
    [[nodiscard]] int fps() const { return m_fps; };

    void on_frame_begin();

    void on_frame_end();

    void limit(int fps) const;
};


#endif //BLINK_FPS_MANAGER_HPP
