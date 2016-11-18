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


#include "seal.h"
#include "math/mat4.h"

#include "camera.h"

EXTERN_GAME;

struct camera* camera_new(float width, float height)
{
    struct camera* c = (struct camera*)s_malloc(sizeof(struct camera));
    c->x = 0.0f;
    c->y = 0.0f;
    c->scale_x = 1.0f;
    c->scale_y = 1.0f;
    c->width = width;
    c->height = height;
    c->dirty = 0;

    mat4_orth(&c->camer_mat, -width/2, -height/2, width/2, height/2, -1, 1);
    return c;
}

void camera_free(struct camera* c)
{
    s_free(c);
}

void camera_pos(struct camera* self, float x, float y)
{
    self->x = x;
    self->y = y;

    self->dirty |= TRANSLATE_DIRTY;
}

void camera_scale(struct camera* self, float sx, float sy)
{
    self->scale_x = 1.0f;
    self->scale_y = 1.0f;

    self->dirty |= SCALE_DIRTY;
}

void camera_update(struct camera* self)
{
    if(!self->dirty) {
        return;
    }

    mat4_translate(&self->camer_mat,
                     (-self->x/self->width) * GAME->glview->__frame_scalar_x,
                     (-self->y/self->height) * GAME->glview->__frame_scalar_y,
                     1.0);

    LOGP("camera traslate x, y = %.2f, %.2f, dx, dy = %.2f, %.2f, w, h = %d, %d",
         self->x, self->y,
         -self->x/(float)self->width,
         -self->y/(float)self->height,
         GAME->config.design_width*2,
         GAME->config.design_height*2);
    self->dirty = 0;
}
