//
// Created by vlado on 25-Jul-18.
//

#include <cstring>
#include <map>
#include "util.h"

unsigned util::loadDDS(const char *filePath) {
    unsigned char header[124];

    FILE *fp;

    /* try to open the file */
    fp = fopen(filePath, "rb");
    if (fp == nullptr) {
        fprintf(stdout, "Couldn't open file.");
        return 0;
    }

    /* verify the type of file */
    char fileCode[4];
    fread(fileCode, 1, 4, fp);
    if (strncmp(fileCode, "DDS ", 4) != 0) {
        fclose(fp);
        fprintf(stdout, "Not a DDS file.");
        return 0;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int *) &(header[8]);
    unsigned int width = *(unsigned int *) &(header[12]);
    unsigned int linearSize = *(unsigned int *) &(header[16]);
    unsigned int mipMapCount = *(unsigned int *) &(header[24]);
    unsigned int fourCC = *(unsigned int *) &(header[80]);

    unsigned char *buffer;
    unsigned int buffSize;
    /* how big is it going to be including all mipmaps? */
    buffSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char *) malloc(buffSize * sizeof(unsigned char));
    fread(buffer, 1, buffSize, fp);
    /* close the file pointer */
    fclose(fp);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
            fprintf(stdout, "Not a valid DXT1 format.");
            return 0;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                               0, size, buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;
    }
    free(buffer);

    return textureID;
}

unsigned util::loadBMP(const char *filePath) {
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char *data;

    // Open the file
    FILE *file = fopen(filePath, "rb");
    if (!file) {
        printf("Image could not be opened\n");
        return 0;
    }

    if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    dataPos = *(unsigned *) &(header[0x0A]);
    imageSize = *(unsigned *) &(header[0x22]);
    width = *(unsigned *) &(header[0x12]);
    height = *(unsigned *) &(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0) dataPos = 54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char[imageSize];

    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return textureID;
}

bool util::loadOBJ(const char *filePath, std::vector<util::Vertex> &out_vertices) {
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    FILE *file = fopen(filePath, "r");
    if (file == nullptr) {
        printf("Couldn't open '%s'!\n", filePath);
        return false;
    }

    while (true) {
        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_positions.push_back(vertex);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int positionIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &positionIndex[0], &uvIndex[0], &normalIndex[0],
                                 &positionIndex[1], &uvIndex[1], &normalIndex[1], &positionIndex[2], &uvIndex[2],
                                 &normalIndex[2]);
            if (matches != 9) {
                printf("Couldn't parse %s\n", filePath);
                return false;
            }

            out_vertices.emplace_back(Vertex(temp_positions[positionIndex[0] - 1], temp_normals[normalIndex[0] - 1],
                                             temp_uvs[uvIndex[0] - 1]));
            out_vertices.emplace_back(Vertex(temp_positions[positionIndex[1] - 1], temp_normals[normalIndex[1] - 1],
                                             temp_uvs[uvIndex[1] - 1]));
            out_vertices.emplace_back(Vertex(temp_positions[positionIndex[2] - 1], temp_normals[normalIndex[2] - 1],
                                             temp_uvs[uvIndex[2] - 1]));
        }
    }

    fclose(file);

    return true;
}

bool util::loadOBJ(const char *filePath, std::vector<util::Vertex> &out_vertices, std::vector<unsigned> &out_indices) {
    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    FILE *file = fopen(filePath, "r");
    if (file == nullptr) {
        printf("Couldn't open '%s'!\n", filePath);
        return false;
    }

    while (true) {
        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_positions.push_back(vertex);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int positionIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &positionIndex[0], &uvIndex[0], &normalIndex[0],
                                 &positionIndex[1], &uvIndex[1], &normalIndex[1], &positionIndex[2], &uvIndex[2],
                                 &normalIndex[2]);
            if (matches != 9) {
                printf("Couldn't parse %s\n", filePath);
                return false;
            }


            for (ust i = 0; i < 3; i++) {

                Vertex v(temp_positions[positionIndex[i] - 1],
                         temp_normals[normalIndex[i] - 1],
                         temp_uvs[uvIndex[i] - 1]);

                bool found = false;
                unsigned j;
                for (j = 0; j < out_vertices.size(); j++) {
                    if (v == out_vertices[j]) {
                        out_indices.push_back(j);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    out_indices.push_back(j);
                }
//
                out_vertices.emplace_back(Vertex(temp_positions[positionIndex[i] - 1],
                                                 temp_normals[normalIndex[i] - 1],
                                                 temp_uvs[uvIndex[i] - 1]));
            }
        }
    }

    fclose(file);

    return true;
}