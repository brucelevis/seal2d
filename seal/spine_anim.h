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

#ifndef __seal__spine_anim__
#define __seal__spine_anim__

#include "seal_base.h"


#if defined (SEAL_USE_SPINE)
struct render;
struct sprite;

struct spine_anim {
    spAtlas* atlas;
    spAnimationState* anim_state;
    spSkeleton* skeleton;

    // TODO: refactor this when we have a better render.
    float vertices[8];
    float uvs[8];
    unsigned char color[4];
};

struct spine_anim* spine_anim_new(const char* atlas_path,
                                  const char* spine_data_path,
                                  float scale);

void spine_anim_free(struct spine_anim* self);

void spine_anim_update(struct spine_anim* self, float dt);

void spine_anim_draw(struct spine_anim* self,
                     struct render* R,
                     float x, float y);

void spine_anim_set_anim(struct spine_anim* self,
                         const char* anim_name,
                         int track,
                         bool loop);

void spine_get_boundingbox(struct spine_anim* self, struct rect* r);

#endif

#endif
