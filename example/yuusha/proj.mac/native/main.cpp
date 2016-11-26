/*
 * Copyright 2011-2016 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bx/uint32_t.h>
#include "common.h"
#include "bgfx_utils.h"

#include "s2_game.h"


class Yuusha : public entry::AppI
{
    void init(int _argc, char** _argv) BX_OVERRIDE
    {
        struct s2_game_config config = {
            "Yuusha", 1024, 768, 0
        };

        s2_game_init(&config);
    }

    virtual int shutdown() BX_OVERRIDE
    {
        
        bgfx::shutdown();

        return 0;
    }

    bool update() BX_OVERRIDE
    {
        struct s2_mat4 ortho;
        if (!entry::processEvents(m_width, m_height, m_debug, m_reset) )
        {
            s2_mat4_orth2(&ortho, 0, m_width, m_height, 0, -1.0f, 1.0f);
            bgfx_touch(0);
            bgfx_set_view_transform(0, NULL, &ortho.m);
            bgfx_set_view_rect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

            s2_node_visit(s2_game_G()->root);


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
    bgfx_texture_handle m_tex_handle;
    bgfx_uniform_handle_t m_tex_uniform_handle;
};

ENTRY_IMPLEMENT_MAIN(Yuusha);
