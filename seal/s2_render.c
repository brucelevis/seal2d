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

#include "s2_render.h"

static const char* s2_sprite_render_get_shader_path()
{
    switch (bgfx_get_renderer_type())
    {
        case BGFX_RENDERER_TYPE_NOOP:
        case BGFX_RENDERER_TYPE_DIRECT3D9:   return "shaders/dx9/";   break;
        case BGFX_RENDERER_TYPE_DIRECT3D11:
        case BGFX_RENDERER_TYPE_DIRECT3D12:  return "shaders/dx11/";  break;
        case BGFX_RENDERER_TYPE_METAL:       return "shaders/metal/"; break;
        case BGFX_RENDERER_TYPE_OPENGL:      return "shaders/glsl/";  break;
        case BGFX_RENDERER_TYPE_OPENGLES:    return "shaders/essl/";  break;

        default:
            s2_assert(false, "unsupport render.");
            break;
    }
}

struct s2_sprite_render* s2_sprite_render_create()
{
    struct s2_sprite_render* render = s2_malloc(sizeof(*render));
    memset(render, 0, sizeof(*render));

    const char* prefix = s2_sprite_render_get_shader_path();

    char vs_name[128] = "";
    char fs_name[128] = "";
    snprintf(vs_name, 128, "%s%s", prefix, "vs_sprite.bin");
    snprintf(fs_name, 128, "%s%s", prefix, "fs_sprite.bin");

    render->__program = s2_program_create(vs_name, fs_name);
    return render;
}

void s2_sprite_render_destroy(struct s2_sprite_render* self)
{
    s2_program_destroy(self->__program);
    s2_free(self);
}
