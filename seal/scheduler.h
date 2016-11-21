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


#ifndef __seal__scheduler__
#define __seal__scheduler__

#include "seal_base.h"

// WC - todo fix


struct Hashmap;
struct array;
struct action;
struct sprite;

struct schedule_entry {
    struct sprite* target;
    struct action* action;
};

struct scheduler {
    struct Hashmap* entries; // key:sprite, value:entry
    float dt;
    float time_scale;

    struct sprite* __removing_target;
//    struct Hashmap* __entries_to_remove;
};

struct scheduler* scheduler_new();
void scheduler_free(struct scheduler* self);

void scheduler_schedule(struct scheduler* self,
                        struct sprite* sprite,
                        struct action* action);

void scheduler_stop_target(struct scheduler* self,
                           struct sprite* sprite);

void scheduler_update(struct scheduler* self, float dt);
void scheduler_pause(struct scheduler* self);
void scheduler_resume(struct scheduler* self);
void scheduler_set_time_scale(struct scheduler* self, float time_scale);

#endif
