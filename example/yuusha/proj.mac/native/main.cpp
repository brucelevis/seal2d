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
        float scale = 0.5f;
        struct s2_game_config config = {
            "Yuusha", (int)(1024 * scale), (int)(768 * scale), 0
        };

        s2_game_init(&game, &config);
    }

    virtual int shutdown() BX_OVERRIDE
    {
        
        bgfx::shutdown();

        return 0;
    }

    bool update() BX_OVERRIDE
    {
        struct s2_mat4 camera;
        if (!entry::processEvents(m_width, m_height, m_debug, m_reset) )
        {
            s2_mat4_orth2(&camera, 0, m_width, 0, m_height, -1.0f, 1.0f);
            bgfx_set_view_transform(0, NULL, &camera.m);
            bgfx_set_view_rect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

            bgfx_touch(0);

            s2_game_update(&game);

            bgfx_dbg_text_clear(0, false);
            bgfx_dbg_text_printf(0, 1, 0x4f, "Yuusha");

            bgfx_frame(0);

            return true;
        }

        s2_game_shutdown(&game);
        
        return false;
    }
    
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;

    struct s2_game game;
};

ENTRY_IMPLEMENT_MAIN(Yuusha);
