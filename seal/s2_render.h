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

#ifndef __s2_render__
#define __s2_render__

#include "s2_common.h"

struct s2_vertex;

struct s2_sprite_render {
    struct s2_program* __program;

    bgfx_vertex_buffer_handle_t __vbh;
    bgfx_index_buffer_handle_t  __ibh;

    struct s2_vertex* __vb;
    uint16_t*         __ib;
};

struct s2_sprite_render* s2_sprite_render_create();
void s2_sprite_render_destroy(struct s2_sprite_render* self);

void s2_sprite_render_draw(struct s2_sprite_render* self, float* model_transform, struct s2_vertex* quad);

#endif /* __s2_render__ */
