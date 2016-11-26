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

#ifndef __seal__mat4__
#define __seal__mat4__

struct s2_mat4 {
    float m[16];
};

void s2_mat4_identify(struct s2_mat4* out);
void s2_mat4_load_translate(struct s2_mat4* out, float x, float y, float z);
void s2_mat4_load_scale(struct s2_mat4* out, float x, float y, float z);

void s2_mat4_orth(struct s2_mat4* out,
               float left,  float bottom,
               float right, float top,
               float near,  float far);

void s2_mat4_translate(struct s2_mat4* out, float x, float y, float z);
void s2_mat4_scale(struct s2_mat4* out, float x, float y, float z);
void s2_mat4_rotate_z(struct s2_mat4* out, float rad);
void s2_mat4_multiply(struct s2_mat4* out, struct s2_mat4* l, struct s2_mat4* r);
void s2_mat4_inverse(struct s2_mat4* in, struct s2_mat4* out);

void s2_mat4_multiply_vec2(const struct s2_mat4* in, float* x, float* y);

void s2_mat4_free(struct s2_mat4* self);
void s2_matrix_debug_print(struct s2_mat4* self);

#endif
