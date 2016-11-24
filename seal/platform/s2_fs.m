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

#include "s2_fs.h"

void s2_fs_read(const char* path, uint8_t** buffer, uint32_t* size)
{
    FILE* fp = fopen(path, "r");
    if (!fp) {
        LOGP("s2_fs_read, can't open file path = %s.\n", path);
        return;
    }

    fseek(fp, 0L, SEEK_END);
    uint32_t file_size = ftell(fp);
    rewind(fp);

    uint8_t* buf = s2_malloc(file_size);
    memset(buf, 0, file_size);
    size_t result = fread(buf, 1, file_size, fp);

    if(result != file_size) {
        free(buf);
        fclose(fp);
        LOGP("s2_fs_read, file reading error, size not match?.\n");
        return;
    }
    if (size) {
        *size = result;
    }

    *buffer = buf;
    LOGI("s2_fs_read read (%s) for (%ld) bytes.", path, result);
    fclose(fp);
}
