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

int lplatform_write_s(lua_State* L)
{
    const char* path = luaL_checkstring(L, -1);
    const char* data = luaL_checkstring(L, -2);
    size_t size = fs_writes(path, data);
    if (size != strlen(data)) {
        luaL_error(L, "write failed, path = %s\n", path);
    }
    return 0;
}

int lplatform_read_s(lua_State* L)
{
    const char* path = luaL_checkstring(L, -1);
    size_t len = 0;
    const char* data = fs_read(path, &len, 0);
    if (data) {
        lua_pushlstring(L, data, len);
        s_free((void*)data);
        return 1;
    }
    return 0;
}

int lplatform_get_write_path(lua_State* L)
{
    lua_pushstring(L, fs_get_write_path());
    return 1;
}

int lplatform_get_sandbox_dir(lua_State* L)
{
    lua_pushstring(L, fs_sandbox_root_path());
    return 1;
}

int lplatform_get_platform(lua_State* L)
{
#ifdef PLAT_MAC
    lua_pushstring(L, "mac");
#elif PLAT_IOS
    lua_pushstring(L, "ios");
#elif PLAT_WIN
    lua_pushstring(L, "win");
#elif PLAT_ANDROID
    lua_pushstring(L, "android");
#endif
    return 1;
}


int lplatform_cmem(lua_State* L)
{
    seal_dump_memory();
    return 0;
}

int lplatform_get_string_for_print(lua_State * L, char* out)
{
    int n = lua_gettop(L);  /* number of arguments */
    int i;

    lua_getglobal(L, "tostring");
    for (i=1; i<=n; i++) {
        const char *s;
        lua_pushvalue(L, -1);  /* function to be called */
        lua_pushvalue(L, i);   /* value to print */
        lua_call(L, 1, 1);
        size_t sz;
        s = lua_tolstring(L, -1, &sz);  /* get result */
        if (s == NULL)
            return luaL_error(L, LUA_QL("tostring") " must return a string to "
                    LUA_QL("print"));
        if (i>1) strcat(out, "\t");
        strncat(out, s, sz);
        lua_pop(L, 1);  /* pop result */
    }
    return 0;
}

int lplatform_lua_print(lua_State * L)
{
#if defined (SDK_DEBUG_LOG)
    char buff[10240] = { 0 };
    lplatform_get_string_for_print(L, buff);
    LOGP_LUA("%s", buff);
#endif
    return 0;
}

int lplatform_lua_loadfile(lua_State * L)
{
    const char* script_path = luaL_checkstring(L, 1);

    size_t buff_len = 0;
    const char* buff = fs_read(script_path, &buff_len, 0);
    luaL_loadbuffer(L, buff, buff_len, script_path);
    s_free(buff);

    return 1;
}

int luaopen_seal_platform(lua_State* L)
{
#ifdef luaL_checkversion
    luaL_checkversion(L);
#endif
    luaL_Reg lib[] = {
        { "read_s", lplatform_read_s },
    #if defined (SEAL_USE_FSWRITE)
        { "write_s", lplatform_write_s },
        { "get_write_path", lplatform_get_write_path },
    #endif
        { "get_sandbox_root_path", lplatform_get_sandbox_dir },
        { "get_platform", lplatform_get_platform },
        { "__print", lplatform_lua_print },
        { "__cmem", lplatform_cmem },
        { "loadfile", lplatform_lua_loadfile },
        { NULL, NULL },
    };

    luaL_newlib(L, lib);
    return 1;
}
