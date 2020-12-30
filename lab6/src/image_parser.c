#include <image_parser.h>
#include <stdio.h>

enum read_status CheckHeader(struct bmp_header header) {
    if (header.bfType != BF_TYPE_DEFAULT) return INPUT_INVALID_SIGNATURE;
    if (header.biBitCount != BI_BIT_COUNT_DEFAULT) return INPUT_INVALID_BITS;
    if (header.biCompression != BI_COMPRESSION_DEFAULT) return INPUT_INVALID_COMPRESSION;

    if (header.bfReserved != BF_RESERVED_DEFAULT) return INPUT_INVALID_HEADER;
    if (header.bfOffBits != BF_OFF_BITS_DEFAULT) return INPUT_INVALID_HEADER;
    if (header.biSize != BI_SIZE_DEFAULT) return INPUT_INVALID_HEADER;
    if (header.biPlanes != BI_PLANES_DEFAULT) return INPUT_INVALID_HEADER;

    return INPUT_OK;

}

size_t GetPaddingBytes(struct Image const * const image) {
    return ((image->width * 3) % 4) * image->height;
}

struct bmp_header ComposeHeader(struct Image const image) {
    struct bmp_header header;

    header.bfType = BF_TYPE_DEFAULT;
    header.bfReserved = BF_RESERVED_DEFAULT;
    header.bfOffBits = BF_OFF_BITS_DEFAULT;
    header.biSize = BI_SIZE_DEFAULT;
    header.biPlanes = BI_PLANES_DEFAULT;

    header.biBitCount = BI_BIT_COUNT_DEFAULT;
    header.biCompression = BI_COMPRESSION_DEFAULT;
    header.biXPelsPerMeter = BI_X_PELS_PER_METER_DEFAULT;
    header.biYPelsPerMeter = BI_Y_PELS_PER_METER_DEFAULT;
    header.biClrUsed = BI_CLR_USED_DEFAULT;
    header.biClrImportant = BI_CLR_IMPORTANT_DEFAULT;

    header.biHeight = image.height;
    header.biWidth = image.width;

    header.biSizeImage = image.width * image.height * 3 + GetPaddingBytes(&image);
    header.bfSize = header.biSizeImage + header.bfOffBits;

    return header;
}

void PrintHeader(struct bmp_header header) {
    printf("         bfType: %#x\n", header.bfType);
    printf("         bfSize: %u\n", header.bfSize);
    printf("     bfReserved: %u\n", header.bfReserved);
    printf("      bfOffBits: %u\n", header.bfOffBits);
    printf("         biSize: %u\n", header.biSize);
    printf("        biWidth: %u\n", header.biWidth);
    printf("       biHeight: %u\n", header.biHeight);
    printf("       biPlanes: %u\n", header.biPlanes);
    printf("     biBitCount: %u\n", header.biBitCount);
    printf("  biCompression: %u\n", header.biCompression);
    printf("    biSizeImage: %u\n", header.biSizeImage);
    printf("biXPelsPerMeter: %u\n", header.biXPelsPerMeter);
    printf("biYPelsPerMeter: %u\n", header.biYPelsPerMeter);
    printf("      biClrUsed: %u\n", header.biClrUsed);
    printf(" biClrImportant: %u\n", header.biClrImportant);

}

const char* ReadEnumToString(enum read_status status) {
    switch (status) {
        case INPUT_OK:
            return "Read ok";
        case INPUT_IMAGE_NULL:
            return "Read image is null";
        case INPUT_INVALID_SIGNATURE:
            return "Read invalid signature";
        case INPUT_INVALID_BITS:
            return "Read invalid bits";
        case INPUT_INVALID_COMPRESSION:
            return "Read invalid compression";
        case INPUT_INVALID_HEADER:
            return "Read invalid header";
        case INPUT_IO_ERROR:
            return "IO error";
        default:
            return "Ok";
    }
}

const char* WriteEnumToString(enum write_status status) {
    switch (status) {
        case OUTPUT_OK:
            return "Write is ok";
        case OUTPUT_IMAGE_NULL:
            return "Write image is null";
        case OUTPUT_IO_ERROR:
            return "Write io error";
        default:
            return "Ok";
    }
}

FILE* OpenFile(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        perror("Can't open file");
    }

    return file;
}

struct Image* Load(const char* filename) {
    FILE* bmpFile;
    uint8_t paddingBytes[4];
    struct bmp_header header;

    bmpFile = OpenFile(filename, "rb");
    if (bmpFile == NULL) {
        return NULL;
    }

    if (!fread(&header, sizeof(header), 1, bmpFile)) {
        perror("Can't read file");
        return NULL;
    }

    enum read_status status = CheckHeader(header);
    if (status != INPUT_OK) {
        printf("Error: %s\n", ReadEnumToString(status));
        return NULL;
    }

    struct Image* image = malloc(sizeof(struct Image));

    image->height = header.biHeight;
    image->width = header.biWidth;
    image->imageData = malloc(image->width * image->height * sizeof(struct Pixel));

    uint16_t bytesToSkip = (image->width * 3) % 4;
    bytesToSkip = (bytesToSkip == 0) ? 0 : (4 - bytesToSkip);

    for (size_t y = 0; y < image->height; y++) {
        size_t pixelsRead = fread(&image->imageData[y * image->width], sizeof(struct Pixel), image->width, bmpFile);
        size_t skipedBytes = fread(paddingBytes, sizeof(uint8_t), bytesToSkip, bmpFile);
        if (!pixelsRead || (bytesToSkip && !skipedBytes)) {
            fclose(bmpFile);
            free(image->imageData);
            return NULL;
        }
    }
    fclose(bmpFile);

    return image;
}

enum write_status Save(const char* filename, struct Image const* image) {
    FILE* bmpFile = OpenFile(filename, "wb");
    uint8_t paddingBytes[4];

    if (image == NULL) return OUTPUT_IMAGE_NULL;

    struct bmp_header header = ComposeHeader(*image);

    if (!fwrite(&header, sizeof(header), 1, bmpFile)) {
        fclose(bmpFile);
        return OUTPUT_IO_ERROR;
    }

    uint16_t bytesToSkip = (image->width * 3) % 4;
    bytesToSkip = (bytesToSkip == 0) ? 0 : (4 - bytesToSkip);

    for (size_t y = 0; y < image->height; y++) {
        size_t pixelsRead = fwrite(&image->imageData[y * image->width], sizeof(struct Pixel), image->width, bmpFile);
        size_t skipedBytes = fwrite(paddingBytes, sizeof(uint8_t), bytesToSkip, bmpFile);
        if (!pixelsRead || (bytesToSkip && !skipedBytes)) {
            fclose(bmpFile);
            free(image->imageData);
            return OUTPUT_IO_ERROR;
        }
    }

    fclose(bmpFile);
    return OUTPUT_OK;
}
