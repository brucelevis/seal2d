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

#include "glview.h"

struct glview* glview_new(int design_w, int design_h, enum design_policy policy)
{
    struct glview* view = STRUCT_NEW(glview);
    view->__design_size.w = design_w;
    view->__design_size.h = design_h;
    view->__policy = policy;

//    view->screen_size = view->__design_size;
    view->fb_size = view->__design_size;
    view->view_size = view->__design_size;

    return view;
}

static void glview_update(struct glview* self)
{
    switch (self->__policy) {
        case FIX_WIDTH:
        {
            break;
        }
        case FIX_HEIGHT:
        {
            break;
        }

        default:
            break;
    }
}

void glview_set_fb_size(struct glview* self, int fb_w, int fb_h)
{
    self->fb_size.w = fb_w;
    self->fb_size.h = fb_h;

    glview_update(self);
}

void glview_set_screen_size(struct glview* self, int screen_w, int screen_h)
{
//    self->screen_size.w = screen_w;
//    self->screen_size.h = screen_h;

//    glview_update(self);
}

void glview_set_view_size(struct glview* self, int view_w, int view_h)
{
    self->view_size.w = view_w;
    self->view_size.h = view_h;

    glview_update(self);
}

void glview_free(struct glview* self)
{
    s_free(self);
}
