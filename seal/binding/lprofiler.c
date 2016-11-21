//
//  lprofiler.c
//  yuusha
//
//  Created by 艺洋 唐 on 11/21/16.
//  Copyright © 2016 levelmax. All rights reserved.
//

#include "../seal.h"

EXTERN_GAME;
int lprofiler_get_fps(lua_State* L)
{
    lua_pushnumber(L, GAME->profiler->fps);
    return 1;
}

int luaopen_seal_profiler(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        { "get_fps", lprofiler_get_fps },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}
