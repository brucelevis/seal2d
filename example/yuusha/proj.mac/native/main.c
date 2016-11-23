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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <float.h>
#include <unistd.h>

#include "timer.h"
#include "window.h"

#include "GLFW/glfw3.h"
#include "test.h"

static void _glfw_error_cb(int error, const char* desc)
{
    fprintf(stderr, "glfw: %s\n", desc);
}

void set_title(GLFWwindow* window, float dt)
{
    static int counter = 0;
    counter++;
    counter = counter % 10;
    if (counter == 0) {
        char title[128] = "";
        if (dt < FLT_EPSILON) {
            dt = 1.0f/60.0f;
        }
        snprintf(title, 128, "%.2f", 1.0f/dt);
        glfwSetWindowTitle(window, title);
    }
}

GLFWwindow* init_glfw(int window_width, int window_height, const char* title)
{
    GLFWwindow* window;
    glfwSetErrorCallback(_glfw_error_cb);

    if (!glfwInit()) {
        fprintf(stderr, "glfwInit failed.\n");
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
    if (!window) {
        fprintf(stderr, "Error, glfwCreateWindow failed.\n");
        glfwTerminate();
        exit(-1);
    }

    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(window, glfw_mouse_pos_callback);
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    const char* vender = (const char*)glGetString(GL_VENDOR);
    const char* render = (const char*)glGetString(GL_RENDERER);
    const char* version = (const char*)glGetString(GL_VERSION);
    const char* shader_version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);


    LOGP("OpenGL:\n \t vender: %s\n"
                   "\t render: %s\n"
                   "\t version: %s\n"
                   "\t shader_version: %s\n",
                            vender,
                            render,
                            version,
                            shader_version);

    return window;
}

void exit_glfw(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main(int argc, char *argv[])
{
    struct s2game* game = seal_load_game_config();

    int window_width = game->config.design_width;
    int window_height = game->config.design_height;

    GLFWwindow* window = init_glfw(window_width,
                                   window_height,
                                   game->config.app_name);
    game->window->ctx = window;
    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

    seal_init_graphics(window_width, window_height, fb_width, fb_height);
    seal_start_game();

#ifdef USE_C_ROOT
    #define TOTAL_SPRITE (10000)
        struct sprite* sprites[TOTAL_SPRITE] = { NULL };
        struct sprite_frame* frame = sprite_frame_cache_get(
                                                        game->sprite_frame_cache,
                                                            "ui.png-bunny.png");

        int speed_x[TOTAL_SPRITE] = {0};
        int speed_y[TOTAL_SPRITE] = {0};
        srand(time(NULL));
        for (int i = 0; i < TOTAL_SPRITE; ++i) {
            struct sprite* s = sprite_new(frame);
            sprite_add_child(game->root, s, 0);
            sprite_set_pos(s, 500, 500);
            sprites[i] = s;

            speed_x[i] = random() % 100;
            speed_y[i] = random() % 100;
        }
#endif

    while (!glfwWindowShouldClose(window)) {
#ifdef USE_C_ROOT
        float dt = game->global_dt;
        for (int i = 0; i < TOTAL_SPRITE; ++i) {
            struct sprite* s = sprites[i];
            float dx = speed_x[i] * dt;
            float dy = speed_y[i] * dt;
            float x = s->x + dx;
            float y = s->y + dy;
            if (x <= 0 || x >= game->config.design_width) {
                speed_x[i] = -speed_x[i];
            }

            if (y <= 0 || y >= game->config.design_height) {
                speed_y[i] = -speed_y[i];
            }
            sprite_set_pos(sprites[i], x, y);
        }
#endif

        seal_update();
        seal_draw();
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    seal_destroy();

    exit_glfw(window);
    return 0;
}
