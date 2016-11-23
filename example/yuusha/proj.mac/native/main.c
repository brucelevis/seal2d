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


#include "GLFW/glfw3.h"
#include <bgfx/c99/bgfx.h>
#include <bgfx/c99/bgfxplatform.h>


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

    bgfx_init(BGFX_PCI_ID_NONE, BGFX_PCI_ID_NONE, 0, NULL, NULL);
    bgfx_reset (fb_width, fb_height, BGFX_RESET_VSYNC);
    bgfx_set_debug(BGFX_DEBUG_TEXT);
    bgfx_set_view_clear (0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0xff3030ff, 1.0f, 0);

    while (!glfwWindowShouldClose(window)) {
        bgfx_set_view_rect(0, 0, 0, window_width, window_height);

        bgfx_touch(0);
        bgfx_dbg_text_clear(0, false);
        bgfx_dbg_text_printf(0, 1, 0x4f, "bgfx/examples/00-helloworld");
        bgfx_dbg_text_printf(0, 2, 0x6f, "Description: Initialization and debug text.");

        // Advance to next frame. Rendering thread will be kicked to
        // process submitted rendering primitives.
        bgfx_frame(0);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    bgfx_shutdown();

    exit_glfw(window);
    return 0;
}
