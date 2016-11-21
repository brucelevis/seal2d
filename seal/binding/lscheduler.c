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

int lscheduler_new(lua_State* L)
{
    struct scheduler* scheduler = scheduler_new();
    lua_pushlightuserdata(L, scheduler);
    return 1;
}

int lscheduler_free(lua_State* L)
{
    struct scheduler* self = (struct scheduler*)lua_touserdata(L, 1);
    scheduler_free(self);
    return 1;
}

int lscheduler_schedule(lua_State* L)
{
    struct scheduler* self = (struct scheduler*)lua_touserdata(L, 1);
    struct sprite* sprite = (struct sprite*)lua_touserdata(L, 2);
    struct action* action = (struct action*)lua_touserdata(L, 3);
    scheduler_schedule(self, sprite, action);
    return 1;
}

int lscheduler_stop_target(lua_State* L)
{
    struct scheduler* self = (struct scheduler*)lua_touserdata(L, 1);
    struct sprite* sprite = (struct sprite*)lua_touserdata(L, 2);
    scheduler_stop_target(self, sprite);
    return 1;
}

int lscheduler_update(lua_State* L)
{
    struct scheduler* self = (struct scheduler*)lua_touserdata(L, 1);
    lua_Number dt = luaL_checknumber(L, 2);
    scheduler_update(self, dt);
    return 1;
}

int luaopen_seal_scheduler(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        { "new", lscheduler_new },
        { "free", lscheduler_free },
        { "schedule", lscheduler_schedule },
        { "stop_target", lscheduler_stop_target },
        { "update", lscheduler_update },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);

    return 1;
}
