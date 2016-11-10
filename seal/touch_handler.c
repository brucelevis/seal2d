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

#include "touch_handler.h"

static void touch_handler_cleanup(struct touch_handler* self)
{
    self->n_visited = 0;
    self->n_touched = 0;
    for (int i = 0; i < MAX_TOUCH_SEQ; ++i)
    {
        self->__visiting[i] = NULL;
        self->__touched[i] = NULL;
    }
}

struct touch_handler* touch_handler_new()
{
    struct touch_handler* self = STRUCT_NEW(touch_handler);
    touch_handler_cleanup(self);
    return self;
}

void touch_handler_free(struct touch_handler* self)
{
    s_free(self);
}

void touch_handler_push(struct touch_handler* self, struct sprite* sprite)
{
    self->__visiting[self->n_visited] = sprite;
    ++self->n_visited;
}

void touch_handler_visit(struct touch_handler* self, struct touch_event* event)
{
    switch (event->type) {
        case TOUCH_BEGIN: {
            for (int i = self->n_visited-1; i >= 0; --i) {
                struct sprite* target = self->__visiting[i];
                if (target) {
                    self->__touched[self->n_visited-(i+1)] = target;
                    ++self->n_touched;
                    sprite_touch(target, event);
                    if (target->swallow) {
                        break;
                    }
                }
            }
            break;
        }

        case TOUCH_MOVE: {
            for (int i = 0; i < self->n_touched; ++i) {
                sprite_touch(self->__touched[i], event);
            }
            break;
        }

        case TOUCH_END:
        case TOUCH_CANCEL:
        {
            for (int i = 0; i < self->n_touched; ++i) {
                sprite_touch(self->__touched[i], event);
            }

            touch_handler_cleanup(self);
            break;
        }

        default:
            break;
    }
}
