/*
 * Copyright (C) 2016 Tang Yiyang
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See BELOW for details.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef __s2_sprite__
#define __s2_sprite__

#include "s2_common.h"

union s2_vec2 {
    struct {
        float x, y;
    };
    struct {
        float w, h;
    };

    float v[2];
};

union s2_color_4b {
    struct {
        uint8_t r,g,b,a;
    };
    uint8_t c[4];
};

struct s2_vertex {
    union s2_vec2 pos;
    union s2_vec2 uv;
    union s2_color_4b color;
};

union s2_rect {
    float x, y;
    float w, h;
};

struct s2_mat4 {
    float m[16];
};

struct s2_node {
    float x, y;
    float anchor_x, anchor_y;
    float scale_x, scale_y;
    float rotation;

    float alpha;
    float visible;
    float touchable;

    float width, height;
    struct s2_mat4 transform;

    struct s2_node* children;
    int children_count;
};

void s2_node_visit(struct s2_node* self);

struct s2_sprite_image {
    struct s2_node __super;

    struct s2_vertex __quad[4];

    struct s2_texture* texture;
    union s2_rect texture_rect;
};

struct s2_sprite_image* s2_sprite_image_create_tex(struct s2_texture* texture);

// @override
void s2_sprite_image_render(struct s2_sprite_image* self);

#endif