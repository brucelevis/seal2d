/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"

#include "s2_game.h"

struct s2_vertex v[4];
uint16_t idx[6];

class Yuusha : public entry::AppI
{


    void init(int _argc, char** _argv) BX_OVERRIDE
    {

        m_width = 1024;
        m_height = 768;
        m_debug = BGFX_DEBUG_TEXT; //| BGFX_DEBUG_STATS;
        m_reset = BGFX_RESET_VSYNC;

        bgfx_init(BGFX_RENDERER_TYPE_COUNT, BGFX_PCI_ID_NONE, 0, NULL, NULL);
        bgfx_reset(m_width, m_height, m_reset);
        bgfx_set_view_clear(0
                            , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                            , 0x303030ff
                            , 1.0f
                            , 0
                            );

        bgfx_set_debug(m_debug);

        
        char vs_name[128] = "";
        char fs_name[128] = "";
        snprintf(vs_name, 128, "%s%s", "shaders/glsl/", "vs_sprite.bin");
        snprintf(fs_name, 128, "%s%s", "shaders/glsl/", "fs_sprite.bin");

        const bgfx_memory_t* vs = s2_fs_read(vs_name);
        const bgfx_memory_t* fs = s2_fs_read(fs_name);

        bgfx_shader_handle_t vs_handle = bgfx_create_shader(vs);
        bgfx_shader_handle_t fs_handle = bgfx_create_shader(fs);

        m_program_handle = bgfx_create_program(vs_handle, fs_handle, true);

//        v[0].pos.x = -0.5f;
//        v[0].pos.y = -0.5f;
//
//        v[1].pos.x = -0.5f;
//        v[1].pos.y = 0.5f;
//
//        v[2].pos.x = 0.5f;
//        v[2].pos.y = -0.5f;
//
//        v[3].pos.x = 0.5f;
//        v[3].pos.y = 0.5f;

        float x = 200;
        float y = 200;
        float w = 100;
        float h = 100;

        for (int i = 0; i < 4; ++i)
        {
            v[i].color.r = 0;
            v[i].color.g = 0;
            v[i].color.b = 0;
            v[i].color.a = 255;
        }

        v[0].pos.x = x;
        v[0].pos.y = y;
        v[0].color.r = 255;

        v[1].pos.x = x;
        v[1].pos.y = y + h;
        v[1].color.g = 255;

        v[2].pos.x = x + w;
        v[2].pos.y = y;
        v[2].color.b = 255;

        v[3].pos.x = x + w;
        v[3].pos.y = y + h;
        v[3].color.r = 0;
        v[3].color.g = 0;
        v[3].color.b = 0;


        idx[0] = 0;
        idx[1] = 1;
        idx[2] = 2;
        idx[3] = 1;
        idx[4] = 3;
        idx[5] = 2;

        bgfx_vertex_decl_begin (&m_vertex_decl, BGFX_RENDERER_TYPE_OPENGL);
        bgfx_vertex_decl_add (&m_vertex_decl, BGFX_ATTRIB_POSITION,  2, BGFX_ATTRIB_TYPE_FLOAT, false, false);
        bgfx_vertex_decl_add (&m_vertex_decl, BGFX_ATTRIB_COLOR0,    4, BGFX_ATTRIB_TYPE_UINT8, true,  false);
        bgfx_vertex_decl_end (&m_vertex_decl);

    }

    virtual int shutdown() BX_OVERRIDE
    {
        bgfx_destroy_program(m_program_handle);
        bgfx::shutdown();

        return 0;
    }

    bool update() BX_OVERRIDE
    {
        if (!entry::processEvents(m_width, m_height, m_debug, m_reset) )
        {
            bgfx_touch(0);

            float ortho[16];
            bx::mtxOrtho(ortho, 0, m_width, m_height, 0, -1.0f, 1.0f);

            bgfx_set_view_transform(0, NULL, ortho);
            bgfx_set_view_rect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));


            bgfx_transient_vertex_buffer tvb;
            bgfx_alloc_transient_vertex_buffer(&tvb, sizeof(v), &m_vertex_decl);

            struct s2_vertex* vertex = (struct s2_vertex*)tvb.data;
            for (int i = 0; i < 4; ++i)
            {
                vertex[i] = v[i];
            }

            bgfx_transient_index_buffer tib;
            bgfx_alloc_transient_index_buffer(&tib, sizeof(idx));

            uint16_t* vi = (uint16_t*)&tib.data;
            for (int i = 0; i < 6; ++i)
            {
                vi[i] = idx[i];
            }

            bgfx_set_state(BGFX_STATE_DEFAULT, 0);


            bgfx_set_transient_vertex_buffer(&tvb, 0, 4);
            bgfx_set_transient_index_buffer(&tib, 0, 6);

            bgfx_submit(0, m_program_handle, 0, false);

            bgfx_dbg_text_clear(0, false);
            bgfx_dbg_text_printf(0, 1, 0x4f, "Yuusha");

            bgfx_frame(0);

            return true;
        }
        
        return false;
    }
    
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;
    bgfx_program_handle_t m_program_handle;
    bgfx_vertex_decl_t m_vertex_decl;
    bgfx_vertex_buffer_handle_t m_vbh;
    bgfx_index_buffer_handle_t m_ibh;
};

ENTRY_IMPLEMENT_MAIN(Yuusha);
