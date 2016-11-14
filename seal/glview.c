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
    float scale_x, scale_y;
    float vw, vh;
    switch (self->__policy) {
        case FIX_WIDTH:
        {
            scale_x = self->__design_size.w / self->view_size.w;
            scale_y = scale_x;

            vw = self->__design_size.w;
            vh = scale_y * self->view_size.h;
            break;
        }
        case FIX_HEIGHT:
        {
            scale_y = self->__design_size.h / self->view_size.h;
            scale_x = scale_y;

            vw = scale_x * self->__design_size.w;
            vh = self->__design_size.h;
            break;
        }

        default:
            break;
    }

    self->view_rect.x = 0;
    self->view_rect.y = 0;
    self->view_rect.w = vw;
    self->view_rect.h = vh;

    LOGP("\nglview_update:\n\t vire_rect = {%d, %d, %d, %d}"
         "\n\t design_resize {%d, %d}"
         "\n\t view_size {%d, %d}"
         "\n",
         self->view_rect.x, self->view_rect.y,
         self->view_rect.w, self->view_rect.h,
         self->__design_size.w, self->__design_size.h,
         self->view_size.w, self->view_size.h);
}

void glview_set_fb_size(struct glview* self, int fb_w, int fb_h)
{
    self->fb_size.w = fb_w;
    self->fb_size.h = fb_h;

    self->__frame_scalar_x = fb_w / self->view_size.w;
    self->__frame_scalar_y = fb_h / self->view_size.h;
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

void glview_update_viewport(struct glview* self)
{
    glViewport(self->view_rect.x, self->view_rect.y, self->view_rect.w, self->view_rect.h);
}

void glview_free(struct glview* self)
{
    s_free(self);
}
