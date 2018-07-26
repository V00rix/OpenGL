//
// Created by vlado on 20-Jul-18.
//

#include <cstring>
#include "util.h"

util::FExtension util::getFileExtension(const char *path) {
    int i = -1;
    while (path[++i] != '\0') {
        if (path[i] == '.') {
            const char* p = &path[i + 1];
            return  strcmp(p, "bmp") == 0 ? util::bmp :
                    strcmp(p, "dds") == 0 ? util::dds : util::unsupported;
        }
    }

    return util::unsupported;
}