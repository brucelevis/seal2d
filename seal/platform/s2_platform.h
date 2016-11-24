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

#ifndef __s2_platform__
#define __s2_platform__

#define SEAL_IOS                1
#define SEAL_ANDROID            2
#define SEAL_MAC                3
#define SEAL_WIN32              4

#ifdef SEAL_PLATFORM_IOS
    #define SEAL_PLATFORM SEAL_IOS
#endif

#ifdef SEAL_PLATFORM_ANDROID
    #define SEAL_PLATFORM SEAL_ANDROID
#endif

#ifdef SEAL_PLATFORM_MAC
    #define SEAL_PLATFORM SEAL_MAC
#endif

#ifdef SEAL_PLATFORM_WIN32
    #define SEAL_PLATFORM SEAL_WIN32
#endif


#endif /* __s2_platform__ */
