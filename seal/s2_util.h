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

#ifndef __s2_util__
#define __s2_util__

#include "s2_common.h"

#if !defined(SEAL_PLATFORM_ANDROID)
    #if defined (SDK_DEBUG_LOG)
        #define LOGI(format, ...)       ((void) printf ("SEAL2D: " format "\n", ##__VA_ARGS__))
        #define LOGW(format, ...)       ((void) printf ("SEAL2D: " format "\n", ##__VA_ARGS__))
        #define LOGV(format, ...)       ((void) printf ("SEAL2D: " format "\n", ##__VA_ARGS__))
        #define LOGP(format, ...)       ((void) printf ("SEAL2D: " format "\n", ##__VA_ARGS__))
        #define LOGP_LUA(format, ...)   ((void) printf ("SEAL2D(LUA): " format "\n", ##__VA_ARGS__))
    #else
        #define LOGI(...) ((void*)0)
        #define LOGW(...) ((void*)0)
        #define LOGV(...) ((void*)0)
        #define LOGP(...)       /**/
        #define LOGP_LUA(...)   /**/
    #endif
#endif

#ifdef DEBUG
    #define s2_assert(e, ...) ((void)((e)|| \
    (LOGP("%s:%d: Assertion failed: %s", __FILE__, (int)__LINE__, #e), abort(), 0)))
#else
    #define s2_assert(e) (0)
#endif

#endif /* __s2_util__ */
