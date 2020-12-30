#include <structs.h>
#include <stdio.h>
#include <math.h>

float gCos = -2;
float gSin = -2;

void InitTrig() {
    if (gCos == -2) gCos = cos(-M_PI / 2);
    if (gSin == -2) gSin = sin(-M_PI / 2);
}

struct Vec2 Rotate(struct Vec2 prev) {
    struct Vec2 new;
    new.x = prev.x * gCos - prev.y * gSin;
    new.y = prev.x * gSin + prev.y * gCos;
    return new;
}

struct Vec2 RotateRelative(struct Vec2 const vec, double const rad) {
    struct Vec2 vec1;
    vec1.x = vec.x * cos(rad) - vec.y * sin(rad);
    vec1.y = vec.x * sin(rad) + vec.y * cos(rad);
    return vec1;
}

void PrintPixel(struct Pixel pixel) {
    printf("Red: %d Green: %d Blue: %d\n", pixel.r, pixel.g, pixel.b);
}

struct Image* RotateImage(struct Image* origImage, int angle) {
    InitTrig();
    double rad = angle * M_PI / 180;
    double pivotX = origImage->width / 2.0;
    double pivotY = origImage->height / 2.0;
    double newPivotX = 0, newPivotY = 0;

    struct Image* rotatedImage = malloc(sizeof(struct Image));

    rotatedImage->height = origImage->height;
    rotatedImage->width = origImage->width;

    rotatedImage->imageData = malloc(rotatedImage->height * rotatedImage->width * 3);

    newPivotX = rotatedImage->width / 2.0;
    newPivotY = rotatedImage->height / 2.0;

    struct Pixel emptyPixel;
    emptyPixel.r = 0;
    emptyPixel.g = 0;
    emptyPixel.b = 0;

    for (int y = 0; y < rotatedImage->height; y++) {
        for (int x = 0; x < rotatedImage->width; x++) {
            struct Vec2 coords;
            double translatedX, translatedY;
            coords.x = x - newPivotX;
            coords.y = y - newPivotY;
            coords = RotateRelative(coords, -rad);
            translatedX = round(coords.x + pivotX);
            translatedY = round(coords.y + pivotY);

            if (translatedX < 0 || translatedX >= origImage->width || translatedY < 0 || translatedY >= origImage->height) {
                rotatedImage->imageData[y * rotatedImage->width + x] = emptyPixel;
            } else {
                rotatedImage->imageData[y * rotatedImage->width + x] = origImage->imageData[(int) translatedY * origImage->width + (int) translatedX];
            }

//            printf("\nOld: %d %d\n", x, y);
//            printf("New: %d %d\n", (int) translatedX, (int) translatedY);
        }
    }

    return rotatedImage;
}

