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

#ifndef __seal__lopen__
#define __seal__lopen__

#include "../seal_base.h"

void stackDump (lua_State* L);

void setfiled_f(lua_State* L, const char* key, float n);
void setfiled_i(lua_State* L, const char* key, lua_Integer n);
void setfiled_s(lua_State* L, const char* key, const char* s);

void setarray_n(lua_State* L, int index, lua_Number n);

lua_Integer getfield_i(lua_State* L, const char* key);
lua_Number  getfield_f(lua_State* L, const char* key);
const char* getfiled_s(lua_State* L, const char* key);

void getarray_f(lua_State* L, const char* key, float* f_array, int cnt);
void getarray_i(lua_State* L, const char* key, int* i_array, int cnt);


#define bind_func_implemention_v(bind, to, ctx) \
    int l##bind(lua_State* L) { \
        to(ctx);\
        return 0; \
    }

#define bind_func_implemention_i1(bind, to, ctx)  \
    int l##bind(lua_State* L) { \
    to(ctx, luaL_checkinteger(L, 1));\
    return 0; \
}

#define bind_func_implemention_f1(bind, to, ctx)  \
    int l##bind(lua_State* L) { \
    to(ctx, luaL_checknumber(L, 1));\
    return 0; \
}

#define bind_func_implemention_f2(bind, to, ctx)  \
    int l##bind(lua_State* L) { \
    to(ctx, luaL_checknumber(L, 1), luaL_checknumber(L, 2));\
    return 0; \
}

#endif
