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

#include "seal.h"

EXTERN_GAME;
#ifdef PLAT_DESKTOP
struct window* win_alloc()
{
    struct window* win = STRUCT_NEW(window);
    win->cursor_x = 0.0f;
    win->cursor_y = 0.0f;
    win->is_touch_down = 0;
    return win;
}

void win_free(struct window* win)
{
    s_free(win);
}

void glfw_mouse_button_callback(GLFWwindow* window,
                                int button, int action, int modify)
{
    switch (action) {
        case GLFW_PRESS: {
                struct touch_event e = {TOUCH_BEGIN,
                                 GAME->window->cursor_x,
                                 GAME->config.design_height - GAME->window->cursor_y,
                                 false};
                seal_touch_event(&e);
                GAME->window->is_touch_down = 1;
            }
            break;

        case GLFW_RELEASE: {
                struct touch_event e = {TOUCH_END,
                    GAME->window->cursor_x,
                    GAME->config.design_height - GAME->window->cursor_y,
                    false};
                seal_touch_event(&e);
                GAME->window->is_touch_down = 0;
            }
            break;

        default:
            break;
    }
}

void glfw_mouse_pos_callback(GLFWwindow* window, double x, double y)
{
    struct window* win = GAME->window;
    win->cursor_x = x;
    win->cursor_y = y;
    if (win->is_touch_down) {
        struct touch_event e = {TOUCH_MOVE,
                                x,
                                GAME->config.design_height - y,
                                false};
        seal_touch_event(&e);
    }
    static char title_text[256] = "";
    snprintf(title_text, 256, "%s:(%d, %d)",
             GAME->config.app_name,
             (int)x, (int)(GAME->config.design_height - y));
    glfwSetWindowTitle(window, title_text);
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key != GLFW_KEY_UNKNOWN) {
        struct key_event e = {key, action};
        seal_event(EVENT_KEY_RECEIVE, on_seal_key_receive, &e);
    }
}

void glfw_framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    LOGP("framebuffer size changed (%d, %d)", w, h);
}

#endif
