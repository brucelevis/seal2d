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

#ifndef __seal__glview__
#define __seal__glview__

#include "seal.h"

enum design_policy {
    FIX_WIDTH = 0,
    FIX_HEIGHT,
};

struct glview {
//    struct size screen_size; // size of real device
    struct size view_size;   // read from the local OpenGLView system(GLKView)
    struct size fb_size;     // framebuffer size

    // these values could NOT be changed during runtime
    struct size __design_size; // the size set by the user, design size.
    enum design_policy __policy; // design policy
    float __view_scalar_x;  // scalar of design/view
    float __view_scalar_y;

    float __frame_scalar_x; // scalear of framebuffer/view (retina support)
    float __frame_scalar_y;
};

struct glview* glview_new(int design_w, int design_h, enum design_policy policy);
void glview_free(struct glview* self);

void glview_set_fb_size(struct glview* self, int fb_w, int fb_h);
//void glview_set_screen_size(struct glview* self, int screen_w, int screen_h);
void glview_set_view_size(struct glview* self, int view_w, int view_h);

#endif /* glview_h */
