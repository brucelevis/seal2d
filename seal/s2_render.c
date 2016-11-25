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

#define SPRITE_RENDER_DEFALT_VERTEX_CAP (2^16)

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

static struct s2_program* s2_sprite_render_load_program()
{
    const char* prefix = s2_sprite_render_get_shader_path();

    char vs_name[128] = "";
    char fs_name[128] = "";
    snprintf(vs_name, 128, "%s%s", prefix, "vs_sprite.bin");
    snprintf(fs_name, 128, "%s%s", prefix, "fs_sprite.bin");

    return s2_program_create(vs_name, fs_name);
}

struct s2_sprite_render* s2_sprite_render_create()
{
    struct s2_sprite_render* render = s2_malloc(sizeof(*render));
    memset(render, 0, sizeof(*render));

    render->__program = s2_sprite_render_load_program();
    render->__vb = s2_malloc(sizeof(struct s2_vertex) * SPRITE_RENDER_DEFALT_VERTEX_CAP);


    bgfx_vertex_decl_t vertex_decl;
    bgfx_vertex_decl_begin (&vertex_decl, BGFX_RENDERER_TYPE_OPENGL);
    bgfx_vertex_decl_add (&vertex_decl, BGFX_ATTRIB_POSITION,  2, BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_decl_add (&vertex_decl, BGFX_ATTRIB_COLOR0,    4, BGFX_ATTRIB_TYPE_UINT8, true,  false);
    bgfx_vertex_decl_add (&vertex_decl, BGFX_ATTRIB_TEXCOORD0, 2, BGFX_ATTRIB_TYPE_FLOAT, false, false);
    bgfx_vertex_decl_end (&vertex_decl);


//    render->__ibh = bgfx_create_vertex_buffer (bgfx_make_ref (s_cubeVertices, sizeof(s_cubeVertices)), &ms_decl, BGFX_BUFFER_NONE);
//    m_ibh = bgfx_create_index_buffer (bgfx_make_ref (s_cubeIndices, sizeof(s_cubeIndices)), BGFX_BUFFER_NONE);

    
    return render;
}

void s2_sprite_render_destroy(struct s2_sprite_render* self)
{
    s2_program_destroy(self->__program);
    s2_free(self);
}
