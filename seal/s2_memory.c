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


#include "s2_memory.h"

struct alloc_entry {
    void *p;
    unsigned long size;
    const char* file;
    int line;
    struct list_head list;
};

static struct list_head* head = NULL;

void s2_memory_add_entry(void* ptr, size_t size, const char* file, int line) {
    struct alloc_entry* entry = (struct alloc_entry*)malloc(sizeof(struct alloc_entry));

    entry->file = file;
    entry->line = line;
    entry->size = size;
    INIT_LIST_HEAD(&entry->list);

    if (!head) {
        static struct list_head l_head;
        INIT_LIST_HEAD(&l_head);
        head = &l_head;
        list_add(&entry->list, &l_head);
    } else {
        list_add_tail(&entry->list, head);
    }

    entry->p = ptr;
}

void s2_memory_delete_entry(void* ptr) {
    struct alloc_entry* iter = NULL;
    struct alloc_entry* found = NULL;

    list_for_each_entry(iter, struct alloc_entry, head, list) {
        if (iter->p == ptr) {
            found = iter;
            break;
        }
    }

    if (found) {
        list_del(&found->list);
        free(found);
    }
}

void* s2_memory_malloc(size_t size, const char* file, int line) {

    void* p = (void*)malloc(size);
    s2_memory_add_entry(p, size, file, line);
    return p;
}

void* s2_memory_calloc(size_t size, const char* file, int line) {
    void* p = s2_memory_malloc(size, file, line);
    memset(p, 0, size);
    return p;
}

void* s2_memory_realloc(void* ptr, size_t size, const char* file, int line) {
    void* p = realloc(ptr, size);
    if (p != ptr) {
        s2_memory_delete_entry(ptr);
        s2_memory_add_entry(p, size, file, line);
    }
    return p;
}

void s2_memory_free(void* ptr) {
    s2_memory_delete_entry(ptr);
    free(ptr);
}

void s2_memory_dump_memory() {
    unsigned long total = 0;

    struct alloc_entry* iter = NULL;

    list_for_each_entry(iter, struct alloc_entry, head, list) {
        total = total + iter->size;
        LOGP("Alloc file = [%s], line = [%d], pointer = [%p]", iter->file, iter->line, iter->p);
        if (iter->line == 200) {
            LOGP("A: type(%d)", *(int*)((((char*)iter->p) + sizeof(unsigned long))));
        }
    }

    LOGP("total allocated memory in [%ld] bytes, [%.2f] MB", total, ((float)total)/(1024*1024));
}
