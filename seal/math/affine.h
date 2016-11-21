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


#ifndef __seal__affine__
#define __seal__affine__

struct mat4;
struct affine {
    float a, b, c, d;
    float x, y;
};

void af_identify(struct affine* out);

void af_srt(struct affine* out,
            float x, float y,
            float scale_x, float scale_y,
            float rotation_x, float rotation_y);

void af_copy(struct affine* out, struct affine* in);

void af_set_translate(struct affine* out, float x, float y);
void af_set_scale(struct affine* out, float scale_x, float scale_y);
void af_set_rotation(struct affine* out, float rotate_x, float rotate_y);

void af_concat(struct affine* out, struct affine* in);

void af_tostring(struct affine* self, char* buff);

void af_transfer_vec2(struct affine* out, float *x, float* y);
void af_transfer_rect(struct affine* out, int *x, int* y, int* w, int* h);
void af_transfer_invert(struct affine* in, struct affine* out);

void af_to_mat4(struct affine* in, struct mat4* mat);

#endif
