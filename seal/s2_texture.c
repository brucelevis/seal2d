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

#include "s2_texture.h"

struct s2_texture* s2_texture_create(const char* path)
{
    const char* p =path + (strlen(path) - 3);
    if (strcmp(p, "png") == 0) {
        struct s2_texture* texture = s2_malloc(sizeof(*texture));
        texture->ref = 1;

        const bgfx_memory_t* mem = s2_fs_read(path);

        int x = 0;
        int y = 0;
        int comp = 0;
        unsigned char* img = stbi_load_from_memory(mem->data, mem->size, &x, &y, &comp, 0);
        texture->__handle = bgfx_create_texture_2d(x, y,
                               false,
                               0,
                               BGFX_TEXTURE_FORMAT_RGBA8,
                               BGFX_TEXTURE_NONE,
                               bgfx_make_ref(img, x*y*comp));

        return texture;
    }

    return NULL;
}

struct s2_texture* s2_texture_retain(struct s2_texture* self)
{
    self->ref++;
    return self;
}

void s2_texture_release(struct s2_texture* self)
{
    self->ref--;
    if(self->ref == 0) {
        s2_texture_destory(self);
    }
}

void s2_texture_destory(struct s2_texture* self)
{
    bgfx_destroy_texture(self->__handle);
    s2_free(self);
}
