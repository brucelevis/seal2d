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

#ifndef __s2_affine__
#define __s2_affine__

struct s2_mat4;

/*
 * -----------------------------------------------------------------------------
 * affine has the folloing matrix layout
 *
 * | a b x |
 * | c d y |
 * | 0 0 1 |
 *
 * -----------------------------------------------------------------------------
 */
struct s2_affine {
    float a, b, c, d;
    float x, y;
};

void s2_affine_identify(struct s2_affine* out);
void s2_affine_set(struct s2_affine* out, float a, float b, float c, float d, float x, float y);
void s2_affine_copy(struct s2_affine* out, struct s2_affine* in);

void s2_affine_set_translate(struct s2_affine* out, float x, float y);
void s2_affine_set_scale(struct s2_affine* out, float scale_x, float scale_y);
void s2_affine_set_rotation(struct s2_affine* out, float rotate_x, float rotate_y);
void s2_affine_concat(struct s2_affine* out, struct s2_affine* in);

void s2_affine_transfer_vec2(struct s2_affine* out, float *x, float* y);
void s2_affine_transfer_rect(struct s2_affine* out, int *x, int* y, int* w, int* h);
void s2_affine_transfer_invert(struct s2_affine* in, struct s2_affine* out);

void s2_affine_to_mat4(struct s2_affine* in, struct s2_mat4* mat);
void s2_affine_to_string(struct s2_affine* self, char* buff);

#endif /* __s2_affine__ */
