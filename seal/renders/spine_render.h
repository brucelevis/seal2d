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

#ifndef spine_render_h
#define spine_render_h

#include "../seal_base.h"

struct spine_render_context {
    struct render_context __super;
    struct vertex_buffer* buffer;
};

struct spine_render_vertex_desc {
    float* vertex;
    float* uv;
    unsigned char* color;
    GLuint tex_id;
    float x, y;
};

void spine_render_batch_reset(struct render_batch* self);

void spine_render_func_init(struct render* R);
void spine_render_destroy(struct render* R);
void spine_render_func_start(struct render* R);
void spine_render_func_draw(struct render* R, void* object);
void spine_render_func_end(struct render* R);
void spine_render_func_flush(struct render* R);

#endif
