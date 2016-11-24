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

#include "s2_program.h"

struct s2_program* s2_program_create(const char* vsh, const char* fsh)
{
    struct s2_program* program = malloc(sizeof(*program));

    const bgfx_memory_t* vs = s2_fs_read(vsh);
    const bgfx_memory_t* fs = s2_fs_read(fsh);

    bgfx_shader_handle_t vs_handle = bgfx_create_shader(vs);
    bgfx_shader_handle_t fs_handle = bgfx_create_shader(fs);

    program->handle = bgfx_create_program(vs_handle, fs_handle, true);

    return program;
}

void s2_program_destroy(struct s2_program* self)
{
    bgfx_destroy_program(self->handle);
}
