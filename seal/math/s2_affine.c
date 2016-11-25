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

#include "s2_common.h"


void s2_affine_identify(struct s2_affine* af)
{
    af->a = 1.0f; af->b = 0.0f;
    af->c = 0.0f; af->d = 1.0f;
    af->x = 0.0f; af->y = 0.0f;
}

void s2_affine_set(struct s2_affine* out, float a, float b, float c, float d, float x, float y)
{
    out->a = a; out->b = b;
    out->c = c; out->d = d;
    out->x = x; out->y = y;
}

void s2_affine_copy(struct s2_affine* out, struct s2_affine* in)
{
    memcpy(out, in, sizeof(struct s2_affine));
}

void s2_affine_set_translate(struct s2_affine* out, float x, float y)
{
    out->x = x;
    out->y = y;
}

void s2_affine_set_scale(struct s2_affine* out, float scale_x, float scale_y)
{
    out->a = scale_x;
    out->d = scale_y;
}

void s2_affine_set_rotation(struct s2_affine* out, float rotate_x, float rotate_y)
{
    float sin = sinf(rotate_x*M_PI/180.0f);
    float cos = cosf(rotate_y*M_PI/180.0f);

    out->a = cos; out->b = -sin;
    out->c = sin; out->d = cos;
}

// [a1, b1, x1]   [a2, b2, x2]   [a1*a2+b1*c2, a1*b2+b1*d2, a1*x2+b1*y2+x1]
// [c1, d1, y1] * [c2, d2, y2] = [c1*a2+d1*c2, c1*b2+d1*d2, c1*x2+d1*y2+y1]
// [0,  0,  1]    [0,  0,  1]    [0,           0,           1             ]
void s2_affine_concat(struct s2_affine* m1, struct s2_affine* m2)
{
    float a = m1->a*m2->a + m1->b*m2->c;
    float b = m1->a*m2->b + m1->b*m2->d;
    float x = m1->x*m2->a + m1->y*m2->c + m2->x;

    float c = m1->c*m2->a + m1->d*m2->c;
    float d = m1->c*m2->b + m1->d*m2->d;
    float y = m1->x*m2->b + m1->y*m2->d + m2->y;

    m1->a = a; m1->b = b; m1->x = x;
    m1->c = c; m1->d = d; m1->y = y;
}

void s2_affine_transfer_vec2(struct s2_affine* af, float*x, float* y)
{
    float ix = *x, iy = *y;
    *x = (float)((double)af->a * ix + (double)af->c * iy + af->x);
    *y = (float)((double)af->b * ix + (double)af->d * iy + af->y);
}

void s2_affine_transfer_rect(struct s2_affine* af, int*x, int* y, int* w, int* h)
{
    float ix = *x, iy = *y, iw = *w, ih = *h;
    float top = iy;
    float left = ix;
    float right = left + iw;
    float bottom = top + ih;

    float tlx = left, tly = top;
    float trx = right, try = top;
    float blx = left, bly = bottom;
    float brx = right, bry = bottom;

    s2_affine_transfer_vec2(af, &tlx, &tly);
    s2_affine_transfer_vec2(af, &trx, &try);
    s2_affine_transfer_vec2(af, &blx, &bly);
    s2_affine_transfer_vec2(af, &brx, &bry);

    float minX = MIN(MIN(tlx, trx), MIN(blx, brx));
    float maxX = MAX(MAX(tlx, trx), MAX(blx, brx));
    float minY = MIN(MIN(tly, try), MIN(bly, bry));
    float maxY = MAX(MAX(tly, try), MAX(bly, bry));

    *x = minX;
    *y = minY;
    *w = maxX - minX;
    *h = maxY - minY;
}

void s2_affine_transfer_invert(struct s2_affine* af, struct s2_affine* out)
{
    float d = 1.0f / (af->a * af->d - af->b * af->c);
    s2_affine_set(out,
                   d *  af->d,
                  -d *  af->b,
                  -d *  af->c,
                   d *  af->a,
                   d * (af->c * af->y - af->d * af->x),
                   d * (af->b * af->x - af->a * af->y));
}

void s2_affine_to_mat4(struct s2_affine* in, struct s2_mat4* mat)
{
    float* m = mat->m;
    m[0] = in->a;
    m[1] = in->b;
    m[2] = 0.0f;
    m[3] = 0.0f;

    m[4] = in->c;
    m[5] = in->d;
    m[6] = 0.0f;
    m[7] = 0.0f;

    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;

    m[12] = in->x;
    m[13] = in->y;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

void s2_affine_to_string(struct s2_affine* af, char* buff)
{
    sprintf(buff, "{a = %.2f, b = %.2f, c = %.2f, d = %.2f, x = %.2f, y = %.2f}",
            af->a, af->b, af->c, af->d, af->x, af->y);
}
