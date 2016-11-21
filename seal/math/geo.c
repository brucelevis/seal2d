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

#include <stdio.h>
#include "geo.h"

bool rect_contains(struct rect* rect, float x, float y)
{
    return !(rect->x                 > x ||
             rect->x+rect->w     < x ||
             rect->y                 > y ||
             rect->y + rect->h  < y );
}

void color_vec4(unsigned long color, float v[4])
{
    v[0] = ((color >> 24) & 0xff) / 255.0f;
    v[1] = ((color >> 16) & 0xff) / 255.0f;
    v[2] = ((color >> 8 ) & 0xff) / 255.0f;
    v[3] = ((color      ) & 0xff) / 255.0f;
}

void color_c4f(unsigned long color, float* r, float* g, float* b, float* a)
{
    *r = ((color >> 24) & 0xff) / 255.0f;
    *g = ((color >> 16) & 0xff) / 255.0f;
    *b = ((color >> 8 ) & 0xff) / 255.0f;
    *a = ((color      ) & 0xff) / 255.0f;
}

void vertex_tostring(char* buff, struct vertex* vertex)
{
    snprintf(buff, 1024, "vertex (%p) { position = {%.2f, %.2f}, color = {%d, %d, %d, %d} }",
                vertex,
                vertex->position[0], vertex->position[1],
                vertex->color[0], vertex->color[1], vertex->color[2], vertex->color[3] );
}
