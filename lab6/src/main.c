#include <image_parser.h>
#include <image_manip.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc < 4) {
        fputs("lab6 <угол> <input.bmp> <output.bmp>\n", stderr);
        return 2;
    }

    struct Image* image = Load(argv[2]);
    if (image == NULL) {
        fputs("404 file not found\n", stderr);
        return 2;
    }

    image = RotateImage(image, strtol(argv[1], NULL, 10));

    if (Save(argv[3], image) != OUTPUT_OK) {
        fputs("Can't save file", stderr);
        return 1;
    }
    printf("Successfull: %s!\n", argv[2]);
    return 0;
}
