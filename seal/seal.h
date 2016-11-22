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


#ifndef __seal__seal__
#define __seal__seal__

#include "seal_base.h"

// project headers - in ALPHABETICAL order
#include "base/array.h"
#include "base/hashmap.h"
#include "base/list.h"
#include "binding/lopen.h"
#include "image/lodepng.h"
#include "math/affine.h"
#include "math/geo.h"
#include "math/mat4.h"
#include "platform/fs.h"
#include "platform/platform.h"
#include "platform/render_opengl.h"
#include "platform/timer.h"
#include "renders/primitive_render.h"
#include "renders/spine_render.h"
#include "renders/sprite_render.h"

#include "action.h"
#include "anim.h"
#include "bmfont.h"
#include "camera.h"
#include "event.h"
#include "glview.h"
#include "lua_handler.h"
#include "memory.h"
#include "render.h"
#include "scheduler.h"
#include "shader.h"
#include "spine_anim.h"
#include "sprite.h"
#include "texture.h"
#include "ttf_font.h"
#include "touch_handler.h"
#include "profiler.h"
#include "util.h"
#include "window.h"

//#define USE_C_ROOT

struct game_config {
    char app_name[128];
    int design_width;
    int design_height;
    int design_policy;
};

struct game {
    lua_State* lstate;

    struct game_config config;

    // core render context
    float global_dt;
    struct camera* global_camera;
    struct glview* glview;
    struct texture_cache* texture_cache;
    struct ttf_font* font;
    struct window* window;           // TODO: move this to glview,
    struct sprite* root;             // the root node of the world
    struct shader* shader;
    struct render* render;
    struct touch_handler* touch_handler;

    // extra util
    struct lua_handler* lua_handler;
    struct sprite_frame_cache* sprite_frame_cache;
    struct bmfont_cache* bmfont_cache;
    struct nuk_node* nuk_node;
    struct scheduler* scheduler;
    struct profiler* profiler;

    struct timeval __last_update;
};

#define EXTERN_GAME extern struct game* GAME;

#define GAME_TRACE       "GAME_TRACE"
#define GAME_INIT        "GAME_INIT"
#define GAME_UPDATE      "GAME_UPDATE"
#define GAME_DRAW        "GAME_DRAW"
#define GAME_PAUSE       "GAME_PAUSE"
#define GAME_RESUME      "GAME_RESUME"
#define GAME_EVENT       "GAME_EVENT"
#define GAME_HANDLERS    "GAME_HANDLERS"

#define EVENT_GAME_START (0)
#define EVENT_GAME_END   (1)
#define EVENT_KEY_RECEIVE (2)

// main game state functions
struct game* seal_load_game_config();
struct glview* seal_init_graphics(int view_w, int view_h, int fb_w, int fb_h);
struct game* seal_game_context();
void seal_load_string(const char* script_data);
void seal_load_file(const char* script_path);
void seal_start_game();
void seal_main_loop();
void seal_update();
void seal_draw();
void seal_destroy();

// Lua
int  seal_call(lua_State *L, int n, int r);
void seal_call_func(void* object,
                    int (*stack_set_func)(lua_State*, void* ud),
                    void* ud,
                    bool cleanup);
void seal_event(int event_type,
                int (*stack_set_func)(lua_State*, void*),
                void* ud);
void seal_touch_event(struct touch_event* e);
void seal_reload_scripts();

int on_seal_key_receive(lua_State* L, void* ud);

#endif
