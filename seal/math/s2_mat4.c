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

/* column matrix.
 * memory index layout.
 * 0, 4, 8,  12,
 * 1, 5, 9,  13,
 * 2, 6, 10, 14,
 * 3, 7, 11, 15,
 */
#define MAT4_SIZE (sizeof(float)* 16)

void s2_mat4_identify(struct s2_mat4* out)
{
    float* m = out->m;
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

void s2_mat4_load_translate(struct s2_mat4* out, float x, float y, float z)
{
    float* m = out->m;
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;
    m[12] = x;
    m[13] = y;
    m[14] = z;
    m[15] = 1.0f;
}

void s2_mat4_load_scale(struct s2_mat4* out, float x, float y, float z)
{
    float* m = out->m;
    m[0] = x;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = y;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = z;
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

void s2_mat4_load_rotate_z(struct s2_mat4* out, float degree)
{
    float c = cos(degree);
    float s = sin(degree);

    float* m = out->m;
    m[0] = c;
    m[1] = s;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = -s;
    m[5] = c;
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

void s2_mat4_orth(struct s2_mat4* out,
               float left,  float bottom,
               float right, float top,
               float nnear,  float nfar)
{
    float* m = out->m;
    m[0] = 2.0f / (right - left);
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;
    m[4] = 0.0f;
    m[5] = 2.0f / (top - bottom);
    m[6] = 0.0f;
    m[7] = 0.0f;
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 2.0f / (nnear - nfar);
    m[11] = 0.0f;
    m[12] = (left + right) / (left - right);
    m[13] = -(top + bottom) / (top - bottom);
    m[14] = (nnear + nfar) / (nnear - nfar);
    m[15] = 1.0f;
}

void s2_mat4_multiply(struct s2_mat4* out, struct s2_mat4* l, struct s2_mat4* r)
{
    float* m1 = l->m;
    float* m2 = r->m;
    float p[16];

    p[0]  = m1[0] * m2[0]  + m1[4] * m2[1] + m1[8]   * m2[2]  + m1[12] * m2[3];
    p[1]  = m1[1] * m2[0]  + m1[5] * m2[1] + m1[9]   * m2[2]  + m1[13] * m2[3];
    p[2]  = m1[2] * m2[0]  + m1[6] * m2[1] + m1[10]  * m2[2]  + m1[14] * m2[3];
    p[3]  = m1[3] * m2[0]  + m1[7] * m2[1] + m1[11]  * m2[2]  + m1[15] * m2[3];

    p[4]  = m1[0] * m2[4]  + m1[4] * m2[5] + m1[8]   * m2[6]  + m1[12] * m2[7];
    p[5]  = m1[1] * m2[4]  + m1[5] * m2[5] + m1[9]   * m2[6]  + m1[13] * m2[7];
    p[6]  = m1[2] * m2[4]  + m1[6] * m2[5] + m1[10]  * m2[6]  + m1[14] * m2[7];
    p[7]  = m1[3] * m2[4]  + m1[7] * m2[5] + m1[11]  * m2[6]  + m1[15] * m2[7];

    p[8]  = m1[0] * m2[8]  + m1[4] * m2[9] + m1[8]   * m2[10] + m1[12] * m2[11];
    p[9]  = m1[1] * m2[8]  + m1[5] * m2[9] + m1[9]   * m2[10] + m1[13] * m2[11];
    p[10] = m1[2] * m2[8]  + m1[6] * m2[9] + m1[10]  * m2[10] + m1[14] * m2[11];
    p[11] = m1[3] * m2[8]  + m1[7] * m2[9] + m1[11]  * m2[10] + m1[15] * m2[11];

    p[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
    p[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
    p[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    p[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

    memcpy(out, p, MAT4_SIZE);
}

void s2_mat4_translate(struct s2_mat4* out, float x, float y, float z)
{
    struct s2_mat4 tmp;
    s2_mat4_load_translate(&tmp, x, y, z);
    s2_mat4_multiply(out, &tmp, out);
}

void s2_mat4_scale(struct s2_mat4* out, float x, float y, float z)
{
    struct s2_mat4 tmp;
    s2_mat4_load_scale(out, x, y, z);
    s2_mat4_multiply(out, &tmp, out);
}

void s2_mat4_rotate_z(struct s2_mat4* out, float rad)
{
    struct s2_mat4 tmp;
    s2_mat4_load_rotate_z(out, rad);
    s2_mat4_multiply(out, &tmp, out);
}

void s2_mat4_inverse(struct s2_mat4* in, struct s2_mat4* out)
{
    float* m = in->m;
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];

    // Calculate the determinant.
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    if (fabs(det) <= FLT_EPSILON) {
        return;
    }

    float* om = out->m;
    float scalar = 1.0f/det;
    om[0]  = scalar * ( m[5]  * b5 - m[6]  * b4 + m[7]  * b3);
    om[1]  = scalar * (-m[1]  * b5 + m[2]  * b4 - m[3]  * b3);
    om[2]  = scalar * ( m[13] * a5 - m[14] * a4 + m[15] * a3);
    om[3]  = scalar * (-m[9]  * a5 + m[10] * a4 - m[11] * a3);

    om[4]  = scalar * (-m[4]  * b5 + m[6]  * b2 - m[7]  * b1);
    om[5]  = scalar * ( m[0]  * b5 - m[2]  * b2 + m[3]  * b1);
    om[6]  = scalar * (-m[12] * a5 + m[14] * a2 - m[15] * a1);
    om[7]  = scalar * ( m[8]  * a5 - m[10] * a2 + m[11] * a1);

    om[8]  = scalar * ( m[4]  * b4 - m[5]  * b2 + m[7]  * b0);
    om[9]  = scalar * (-m[0]  * b4 + m[1]  * b2 - m[3]  * b0);
    om[10] = scalar * ( m[12] * a4 - m[13] * a2 + m[15] * a0);
    om[11] = scalar * (-m[8]  * a4 + m[9]  * a2 - m[11] * a0);

    om[12] = scalar * (-m[4]  * b3 + m[5]  * b1 - m[6]  * b0);
    om[13] = scalar * ( m[0]  * b3 - m[1]  * b1 + m[2]  * b0);
    om[14] = scalar * (-m[12] * a3 + m[13] * a1 - m[14] * a0);
    om[15] = scalar * ( m[8]  * a3 - m[9]  * a1 + m[10] * a0);
}

void s2_mat4_multiply_vec2(const struct s2_mat4* in, float* x, float* y)
{
    const float* om = in->m;
    float tx = *x;
    float ty = *y;

    *x = om[0] * tx + om[1] * ty;
    *y = om[4] * tx + om[5] * ty;
}

void s2_mat4_free(struct s2_mat4* self)
{
    s2_free(self);
}

void s2_matrix_debug_print(struct s2_mat4* self)
{
    float* m = self->m;
    LOGP("------------------------------------------------\n");
    LOGP("|\t %.5f,\t %.5f,\t %.5f,\t %.5f\t |\n", m[0], m[4], m[8], m[12]);
    LOGP("|\t %.5f,\t %.5f,\t %.5f,\t %.5f\t |\n", m[1], m[5], m[9], m[13]);
    LOGP("|\t %.5f,\t %.5f,\t %.5f,\t %.5f\t |\n", m[2], m[6], m[10], m[14]);
    LOGP("|\t %.5f,\t %.5f,\t %.5f,\t %.5f\t |\n", m[3], m[7], m[11], m[15]);
    LOGP("------------------------------------------------\n");
}
