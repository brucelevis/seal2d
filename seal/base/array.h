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

#ifndef __seal__array__
#define __seal__array__

#include "../seal_base.h"


struct array;

struct  array* array_new(int cap);
void    array_free(struct array* a);

struct array* array_copy(struct array* self);

void    array_push_back(struct array* self, void* data);
void    array_remove(struct array* self, void* data);
void    array_set(struct array* self, size_t index, void* data);
void*   array_at(struct array* self, size_t index);
void    array_swap(struct array* self, int i, int j);

// if free is set to none-zero, it will call s_free to dealloc the memory
// allocated by the user.
void    array_clear(struct array* self, bool cleanup);
int     array_empty(struct array* self);
size_t  array_size(struct array* self);

void*   array_data(struct array* self);

void array_debug_print(struct array* self);

#endif
