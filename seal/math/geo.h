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

#ifndef __seal__geo__
#define __seal__geo__

#include <stdbool.h>
#include "../platform/render_opengl.h"

typedef unsigned long color;

struct vertex {
    GLfloat position[2]; // x, y
    GLubyte color[4];   // r, g, b, a
    GLfloat uv[2];        // u, v
};

struct primitive_vertex {
    GLfloat position[2];
    GLubyte color[4];
};

#define SET_VERTEX_POS(vert, x, y) \
do { \
vert.position[0] = x; \
vert.position[1] = y; \
} while(0);

#define SET_VERTEX_COLOR(vert, r, g, b, a) \
do { \
vert.color[0] = r; \
vert.color[1] = g; \
vert.color[2] = b; \
vert.color[3] = a; \
} while(0);

#define SET_VERTEX_UV(vert, u, v) \
do { \
vert.uv[0] = u; \
vert.uv[1] = v; \
} while(0);

#define SET_VERTEX_COLOR_UINT(vert, c) \
do { \
vert.color[0] = (c >> 24) & 0xff; \
vert.color[1] = (c >> 16) & 0xff; \
vert.color[2] = (c >> 8 ) & 0xff; \
vert.color[3] = (c      ) & 0xff; \
} while(0);

#define VERTEX_SIZE (sizeof(struct vertex))
#define VERTEX_OFFSET_POS ((void*)offsetof(struct vertex, position))
#define VERTEX_OFFSET_COLOR ((void*)offsetof(struct vertex, color))
#define VERTEX_OFFSET_UV ((void*)offsetof(struct vertex, uv))

#define PRIMITIVE_VERTEX_SIZE (sizeof(struct primitive_vertex))
#define PRIMITIVE_VERTEX_OFFSET_POS ((void*)offsetof(struct primitive_vertex, position))
#define PRIMITIVE_VERTEX_OFFSET_COLOR ((void*)offsetof(struct primitive_vertex, color))


#define C4B_COLOR(r,g,b,a) ( (r<<24) | (g<<16) | (b<<8) | (a<<0) )

#define C4B_COLOR_A(arr) C4B_COLOR(arr[0], arr[1], arr[2], arr[3])

#define COLOR_OPACITY(color) (color & 0xff)
#define COLOR_OPACITY_F(color) (COLOR_OPACITY(color)/255.0f)

struct glyph {
    struct vertex tl;
    struct vertex bl;
    struct vertex tr;
    struct vertex br;

    GLuint tex_id;
};

struct rect {
    int x, y;
    int w, h;
};

struct uv {
    float u,v;
    float w,h;
};

struct size {
    int w, h;
};

bool rect_contains(struct rect* rect, float x, float y);

void color_vec4(unsigned long color, float v[4]);
void color_c4f(unsigned long color, float* r, float* g, float* b, float* a);
void vertex_tostring(char* buff, struct vertex* vertex);

#endif /* defined(__yuusha__geo__) */
