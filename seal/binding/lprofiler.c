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
