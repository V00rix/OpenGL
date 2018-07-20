//
// Created by vlado on 20-Jul-18.
//

#ifndef OPENGL_UTIL_H
#define OPENGL_UTIL_H
#define FOURCC_DXT1 827611204 // 'D' 'X' 'T' '1'
#define FOURCC_DXT3 861165636 // 'D' 'X' 'T' '3'
#define FOURCC_DXT5 894720068 // 'D' 'X' 'T' '5'

namespace util {

    /**
     * Load DDS texture into memory
     * @param imagePath
     * @return
     */
    unsigned loadDDS(const char *imagePath);
}
#endif //OPENGL_UTIL_H
