//
//  profiler.c
//  yuusha
//
//  Created by 艺洋 唐 on 11/21/16.
//  Copyright © 2016 levelmax. All rights reserved.
//

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
