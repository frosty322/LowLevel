#pragma once

#include <stdlib.h>
#include <stdint.h>

#define     BI_BIT_COUNT_DEFAULT 24
#define     BI_COMPRESSION_DEFAULT 0
#define     BI_X_PELS_PER_METER_DEFAULT 0
#define     BI_Y_PELS_PER_METER_DEFAULT 0
#define     BI_CLR_USED_DEFAULT 0
#define     BI_CLR_IMPORTANT_DEFAULT 0

#define     BF_TYPE_DEFAULT 0x4d42 /* BMP type, little-endian */
#define     BF_RESERVED_DEFAULT 0
#define     BF_OFF_BITS_DEFAULT 54 /* total BMPv3 header size */
#define     BI_PLANES_DEFAULT 1
#define     BI_SIZE_DEFAULT 40 /* total size of BMPv3 specific values */

enum read_status {
    INPUT_OK,
    INPUT_IMAGE_NULL,
    INPUT_INVALID_SIGNATURE,
    INPUT_INVALID_BITS,
    INPUT_INVALID_COMPRESSION,
    INPUT_INVALID_HEADER,
    INPUT_IO_ERROR
};

enum write_status {
    OUTPUT_OK = 0,
    OUTPUT_IMAGE_NULL,
    OUTPUT_IO_ERROR
};


struct Pixel {
    uint8_t r, g, b;
};

struct Image {
    size_t width, height;
    struct Pixel* imageData;
};

struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfSize;
    uint32_t bfReserved;
    uint32_t bfOffBits;
    uint32_t biSize;

    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

struct Vec2 {
    float x;
    float y;
};

void PrintPixel(struct Pixel pixel);

