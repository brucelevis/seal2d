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
#include "lua.h"

// core engine part
extern int luaopen_seal_core(lua_State* L);
extern int luaopen_seal_platform(lua_State* L);
extern int luaopen_seal_texture(lua_State* L);
extern int luaopen_seal_sprite(lua_State* L);
extern int luaopen_seal_action(lua_State* L);
extern int luaopen_seal_scheduler(lua_State* L);
extern int luaopen_seal_profiler(lua_State* L);

// third part part.
#if defined (SEAL_USE_LUASOCKET)
extern int luaopen_socket_core(lua_State *L);
extern int luaopen_mime_core(lua_State *L);
#endif
extern int luaopen_cjson(lua_State* L);
extern int luaopen_zlib(lua_State* L);

extern int luaopen_nuklear_core(lua_State* L);
extern int luaopen_nanovg_core(lua_State* L);

void stackDump (lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

            case LUA_TSTRING:  /* strings */
                LOGP_LUA("`%s'", lua_tostring(L, i));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                LOGP_LUA("%s", lua_toboolean(L, i) ? "true" : "false");
                break;

            case LUA_TNUMBER:  /* numbers */
                LOGP_LUA("%g", lua_tonumber(L, i));
                break;

            default:  /* other values */
                LOGP_LUA("%s", lua_typename(L, t));
                break;

        }
        LOGP_LUA("    ");  /* put a separator */
    }
    LOGP_LUA("\n");  /* end the listing */
}

void setfiled_f(lua_State *L, const char* key, float f)
{
    lua_pushstring(L, key);
    lua_pushnumber(L, f);
    lua_settable(L, -3);
}

void setfiled_i(lua_State* L, const char* key, lua_Integer n)
{
    lua_pushstring(L, key);
    lua_pushinteger(L, n);
    lua_settable(L, -3);
}

void setfiled_s(lua_State *L, const char* key, const char* s)
{
    lua_pushstring(L, key);
    lua_pushstring(L, s);
    lua_settable(L, -3);
}

void setarray_n(lua_State* L, int index, lua_Number n)
{
    lua_pushinteger(L, index);
    lua_pushnumber(L, n);
    lua_settable(L, -3);
}

lua_Integer getfield_i(lua_State* L, const char* key)
{
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    lua_Integer value = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return value;
}

lua_Number getfield_f(lua_State* L, const char* key)
{
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    lua_Integer value = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return value;
}

const char* getfiled_s(lua_State* L, const char* key)
{
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    const char* value = lua_tostring(L, -1);
    lua_pop(L, 1);
    return value;
}

void getarray_f(lua_State* L, const char* key, float* f_array, int cnt)
{
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if(!lua_isnil(L, -1)) {
        for (int i = 0; i < cnt; ++i) {
            lua_rawgeti(L, -1, i+1);
            f_array[i] = lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
}

void getarray_i(lua_State* L, const char* key, int* f_array, int cnt)
{
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (!lua_isnil(L, -1)) {
        for (int i = 0; i < cnt; ++i) {
            lua_rawgeti(L, -1, i+1);
            f_array[i] = lua_tointeger(L, -1);
            lua_pop(L, 1);
        }
    }
    lua_pop(L, 1);
}

void luaopen_lua_extensions(lua_State *L)
{
    // load extensions
    luaL_Reg lua_modules[] = {
        { "engine_core",    luaopen_seal_core },
        { "platform_core",  luaopen_seal_platform },
        { "texure_core",    luaopen_seal_texture },
        { "sprite_core",    luaopen_seal_sprite },
        { "action_core",    luaopen_seal_action },
        { "scheduler_core", luaopen_seal_scheduler },
        { "profiler_core",  luaopen_seal_profiler },

    #if defined (SEAL_USE_LUASOCKET)
        { "socket.core",    luaopen_socket_core },
        { "mime.core",      luaopen_mime_core },
    #endif
        { "cjson",          luaopen_cjson },
        { "zlib",           luaopen_zlib },

        { "nuklear_core",   luaopen_nuklear_core },
    #ifdef PLAT_DESKTOP
        { "nanovg_core",    luaopen_nanovg_core },
    #endif

        {NULL, NULL}
    };

    luaL_Reg* lib = lua_modules;
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    for (; lib->func; lib++) {
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
    }

    lua_pop(L, 2);
}
