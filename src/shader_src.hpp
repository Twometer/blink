//
// Created by Twometer on 2 Feb 2022.
//

#ifndef BLINK_SHADER_SRC_HPP
#define BLINK_SHADER_SRC_HPP

static const char* const SHADER_BASIC_VERT = R"|(
#version 330 core

layout (location = 0) in vec4 vertPosTex;

out vec2 fragTexCoord;

uniform mat4 mvpMatrix;
uniform vec4 rect;

void main() {
    vec2 pos = vertPosTex.xy;
    pos *= rect.zw;
    pos += rect.xy;
    gl_Position = mvpMatrix * vec4(pos, 0.0, 1.0);
    fragTexCoord = vertPosTex.zw;
}
)|";

static const char* const SHADER_BASIC_FRAG = R"|(
#version 330 core

in vec2 fragTexCoord;

uniform sampler2D tex;

out vec4 fragColor;

void main() {
    float alpha = texture(tex, fragTexCoord).r;
    fragColor = vec4(1, 1, 1, alpha);
}
)|";

#endif //BLINK_SHADER_SRC_HPP
