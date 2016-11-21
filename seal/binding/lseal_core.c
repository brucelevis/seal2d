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

#include "../seal.h"

extern void stackDump (lua_State *L);

int lsealinject(lua_State* L)
{
    if(!lua_istable(L, -1)) {
        LOGP_LUA("seal.start require a table to start.");
        exit(1);
    }

    lua_getfield(L, -1, "init");
    lua_setfield(L, LUA_REGISTRYINDEX, GAME_INIT);

    lua_getfield(L, -1, "update");
    lua_setfield(L, LUA_REGISTRYINDEX, GAME_UPDATE);

    lua_getfield(L, -1, "draw");
    lua_setfield(L, LUA_REGISTRYINDEX, GAME_DRAW);

    lua_getfield(L, -1, "pause");
    lua_setfield(L, LUA_REGISTRYINDEX, GAME_PAUSE);

    lua_getfield(L, -1, "resume");
    lua_setfield(L, LUA_REGISTRYINDEX, GAME_RESUME);

    lua_getfield(L, -1, "event");
    lua_setfield(L, LUA_REGISTRYINDEX, GAME_EVENT);

    lua_pop(L, -1);
    return 0;
}

int lseal_reload_script(lua_State* L)
{
    seal_reload_scripts();
    return 0;
}

int luaopen_seal_core(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        { "inject",     lsealinject},
        { "reload_script", lseal_reload_script},
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}
