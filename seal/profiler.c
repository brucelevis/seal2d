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

#include "profiler.h"

struct profiler* profiler_new()
{
    struct profiler* p = STRUCT_NEW(profiler);
    memset(p, 0, sizeof(struct profiler));
    return p;
}

void profiler_free(struct profiler* self)
{
    s_free(self);
}

void profiler_tick(struct profiler* self, float dt)
{
    if (self->n_ticks == PROFILE_FPS_TICK) {
        self->n_ticks = 0;
        float t = 0.0f;
        for (int i = 0; i < PROFILE_FPS_TICK; ++i) {
            t += self->counter[i];
        }
        self->fps = 1 / (t/PROFILE_FPS_TICK);
    } else {
        self->counter[self->n_ticks++] = dt;
    }
}
