//
// Created by Twometer on 13 Apr 2022.
//

#ifndef BLINK_ATLAS_HPP
#define BLINK_ATLAS_HPP

#include <vector>
#include <map>
#include <glad/glad.h>
#include <optional>

struct atlas_sprite {
    float u;
    float v;
    float w;
    float h;
};

struct atlas_row {
    const int y_pos;
    const int height;
    int offset;
};

class atlas {

private:
    int m_size;
    int m_min_row_size;
    GLuint m_texture_id = 0;
    std::vector<atlas_row> m_rows{};
    std::map<int, atlas_sprite> m_sprites{};

public:
    atlas(int size, int min_row_size);

    const atlas_sprite & insert(int sprite_id, int w, int h, uint8_t *data);

    [[nodiscard]] inline const atlas_sprite &get_sprite(int sprite_id) const { return m_sprites.at(sprite_id); }

    [[nodiscard]] inline int size() const { return m_size; }

    [[nodiscard]] inline GLuint texture_id() const { return m_texture_id; }

private:
    atlas_row &find_free_row(int w, int h);

};


#endif //BLINK_ATLAS_HPP
