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
    struct s2_texture* texture = s2_malloc(sizeof(*texture));

    const bgfx_memory_t* mem = s2_fs_read(path);
    bgfx_texture_info_t info;

    texture->__handle = bgfx_create_texture(mem, BGFX_TEXTURE_NONE, 0, &info);
    texture->width = info.width;
    texture->height = info.height;

    return texture;
}

void s2_texture_destory(struct s2_texture* self)
{
    bgfx_destroy_texture(self->__handle);
    s2_free(self);
}
