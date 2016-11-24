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

#ifndef __s2_memory__
#define __s2_memory__

#include "s2_common.h"

#if DEBUG
// memory hook functions, these functions are SLOW.
// use `if !DEBUG' if you need do the performance test.
#define s2_malloc(size)          s2_memory_malloc(size, __FILE__, __LINE__)
#define s2_calloc(size)          s2_memory_calloc(size, __FILE__, __LINE__)
#define s2_realloc(p, size)      s2_memory_realloc(p, size, __FILE__, __LINE__)
#define s2_free(p)               s2_memory_free((p))

#else

#define s2_malloc    malloc
#define s2_calloc    calloc
#define s2_realloc   realloc
#define s2_free      free

#endif


#define STRUCT_NEW(type) (struct type*)s_malloc(sizeof(struct type))

void s2_memory_add_entry(void* ptr, size_t size, const char* file, int line);
void s2_memory_delete_entry(void* ptr);

extern void* s2_memory_malloc(size_t size, const char* file, int line);
extern void* s2_memory_calloc(size_t size, const char* file, int line);
extern void* s2_memory_realloc(void* ptr, size_t size, const char* file, int line);
extern void  s2_memory_free(void* ptr);
extern void  s2_memory_dump_memory();

#endif /* __s2_memory__ */
