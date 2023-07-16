#pragma once

#include "S3DLVector3.h"
#include "S3DLArray.h"
#include <cstdio>
// Class that can contain a picture and save it to disk as PNG
class S3DLColorPicture
{
public:
    // S3DLColorPicture();
    // ~S3DLColorPicture();

    // Set size of the picture
    void resize(const int width, const int height)
    {
        _width = width;
        _height = height;
        _picture.resize(width * height);
    }

    // Set color of the pixel
    void SetAt(const int x, const int y, const S3DLRGB & color)
    {
        _picture[y * _width + x] = color;
    }

    // Save picture to disk
    void Save(const char * filename)
    {
        // Open file
        FILE * file = fopen(filename, "wb");
        if(!file) {
            printf("Could not write file\n");
            return;
        }

        // Write header
        unsigned char header[54] = {
            0x42, 0x4D, // BMP signature
            0, 0, 0, 0, // Size of BMP file
            0, 0, // Reserved
            0, 0, // Reserved
            54, 0, 0, 0, // Offset of pixel data inside BMP file
            40, 0, 0, 0, // Size of BITMAPINFOHEADER structure
            0, 0, 0, 0, // Width of picture
            0, 0, 0, 0, // Height of picture
            1, 0, // Number of color planes
            24, 0, // Number of bits per pixel
            0, 0, 0, 0, // Compression
            0, 0, 0, 0, // Size of image
            0, 0, 0, 0, // Horizontal resolution
            0, 0, 0, 0, // Vertical resolution
            0, 0, 0, 0, // Number of colors in palette
            0, 0, 0, 0, // Number of important colors
        };
        int const size = _width * _height * 3;
        int const fileSize = size + sizeof(header);
        header[2] = (unsigned char)(fileSize);
        header[3] = (unsigned char)(fileSize >> 8);
        header[4] = (unsigned char)(fileSize >> 16);
        header[5] = (unsigned char)(fileSize >> 24);
        header[18] = (unsigned char)(_width);
        header[19] = (unsigned char)(_width >> 8);
        header[20] = (unsigned char)(_width >> 16);
        header[21] = (unsigned char)(_width >> 24);
        header[22] = (unsigned char)(_height);
        header[23] = (unsigned char)(_height >> 8);
        header[24] = (unsigned char)(_height >> 16);
        header[25] = (unsigned char)(_height >> 24);
        fwrite(header, 1, sizeof(header), file);
        fwrite(_picture.data(), 1, size, file);
        // write data BGR
        // for(int y = 0; y < _height; ++y) {
        //     for(int x = 0; x < _width; ++x) {
        //         S3DLRGB const color = _picture[y * _width + x];
        //         unsigned char pixel[3] = {
        //             (unsigned char)(color.b * 255),
        //             (unsigned char)(color.g * 255),
        //             (unsigned char)(color.r * 255),
        //         };
        //         fwrite(pixel, 1, 3, file);
        //     }
        // }
        fclose(file);
    }

    // width of the picture
    int width() const
    {
        return _width;
    }

    // height of the picture
    int height() const
    {
        return _height;
    }

private:
    // Pointer to the picture
    S3DLArray<S3DLRGB> _picture;

    // Size of the picture
    int _width;
    int _height;
};
