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
            "Yuusha", 1280, 720, 0
        };
        s2_game_init(&config);
    }

    virtual int shutdown() BX_OVERRIDE
    {
        // Shutdown bgfx.
        bgfx::shutdown();

        return 0;
    }

    bool update() BX_OVERRIDE
    {
        if (!entry::processEvents(m_width, m_height, m_debug, m_reset) )
        {
            // Set view 0 default viewport.
            bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
//
//            // This dummy draw call is here to make sure that view 0 is cleared
//            // if no other draw calls are submitted to view 0.
//            bgfx::touch(0);
//
//            // Use debug font to print information about this example.
//            bgfx::dbgTextClear();
//            bgfx::dbgTextImage(bx::uint16_max(uint16_t(m_width /2/8 ), 20)-20
//                               , bx::uint16_max(uint16_t(m_height/2/16), 6)-6
//                               , 40
//                               , 12
//                               , s_logo
//                               , 160
//                               );
//            bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/00-helloworld");
//            bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Initialization and debug text.");
//            
//            // Advance to next frame. Rendering thread will be kicked to
//            // process submitted rendering primitives.
//            bgfx::frame();

            return true;
        }
        
        return false;
    }
    
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;
};

ENTRY_IMPLEMENT_MAIN(Yuusha);
