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


static int hash_str(void* key)
{
    return hashmapHash(key, sizeof(void*));
}

static bool hash_equal(void* a, void* b)
{
    return a == b;
}

struct scheduler* scheduler_new()
{
    struct scheduler* s = STRUCT_NEW(scheduler);
    s->entries = hashmapCreate(32, hash_str, hash_equal);
    s->time_scale = 1.0f;

    s->__removing_target = NULL;
    return s;
}

void scheduler_free(struct scheduler* self)
{
    hashmapFree(self->entries);
    s_free(self);
}

void scheduler_schedule(struct scheduler* self,
                        struct sprite* sprite,
                        struct action* action)
{
    action_play(action, sprite);

    struct schedule_entry* entry = STRUCT_NEW(schedule_entry);
    entry->action = action;
    entry->target = sprite;
    hashmapPut(self->entries, sprite, entry);
}

void scheduler_stop_target(struct scheduler* self,
                           struct sprite* sprite)
{
    LOGP("stop target = sprite_id = %d", sprite->__id);

    struct schedule_entry* e = hashmapGet(self->entries, sprite);
    if(e) {
        action_free(e->action);
        hashmapRemove(self->entries, sprite);
    }
}

static bool scheduler_entries_update(void* key, void* value, void* context)
{
    struct schedule_entry* entry = (struct schedule_entry*)value;
    struct scheduler* self = (struct scheduler*)context;
    struct action* act = entry->action;
    if(action_update(act, entry->target, self->dt)) {
        hashmapRemove(self->entries, entry->target);
        action_free(entry->action);
    }

    return true;
}

void scheduler_update(struct scheduler* self, float dt)
{
    self->dt = dt * self->time_scale;
    hashmapForEach(self->entries, scheduler_entries_update, self);
}

void scheduler_pause(struct scheduler* self)
{

}

void scheduler_resume(struct scheduler* self)
{

}

void scheduler_set_time_scale(struct scheduler* self, float time_scale)
{
    self->time_scale = time_scale;
}
