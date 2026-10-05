/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#include "OpenGL.h"
#include "SaveScreenshotBMP.h"

#pragma pack(push, 1)
typedef struct BitmapFileHeader 
{
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
} WinCoreBitmapFileHeader;

typedef struct BitmapInfoHeader 
{
    unsigned int   biSize;
    int            biWidth;
    int            biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} WinCoreBitmapInfoHeader;
#pragma pack(pop)

void SaveScreenshotBMP(const char* filename, int width, int height)
{
    FILE* file;
    WinCoreBitmapFileHeader bmfh;
    WinCoreBitmapInfoHeader bmih;
    unsigned char* pixels;
    int row_size;
    int i;

    row_size = (width * 3 + 3) & ~3;

    bmfh.bfType = 0x4D42;
    bmfh.bfSize = sizeof(WinCoreBitmapFileHeader) + sizeof(WinCoreBitmapInfoHeader) + (row_size * height);
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = sizeof(WinCoreBitmapFileHeader) + sizeof(WinCoreBitmapInfoHeader);

    bmih.biSize = sizeof(WinCoreBitmapInfoHeader);
    bmih.biWidth = width;
    bmih.biHeight = height;
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;
    bmih.biCompression = 0;
    bmih.biSizeImage = row_size * height;
    bmih.biXPelsPerMeter = 0;
    bmih.biYPelsPerMeter = 0;
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;

    pixels = (unsigned char*)malloc(row_size * height);
    if (!pixels)
    {
        return;
    }

    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    file = fopen(filename, "wb");
    if (file)
    {
        fwrite(&bmfh, sizeof(WinCoreBitmapFileHeader), 1, file);
        fwrite(&bmih, sizeof(WinCoreBitmapInfoHeader), 1, file);
        fwrite(pixels, row_size * height, 1, file);
        fclose(file);
    }

    free(pixels);
}