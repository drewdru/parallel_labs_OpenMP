#include <iostream>
#include <omp.h>
#include <fstream>
#include <iomanip>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include "Image_reader.h"

using namespace std;

void readBMP(string fileName, BITMAPFILEHEADER &header, \
             BITMAPINFOHEADER &bmiHeader, RGBQUAD **&rgb);
void writeBMP(string fileName, BITMAPFILEHEADER header, \
              BITMAPINFOHEADER bmiHeader, RGBQUAD **rgb);
void medianfilter(BITMAPINFOHEADER bmiHeader, int **&temp, int RH, int RW);
void ZapMedMas(BITMAPINFOHEADER bmiHeader, int **temp, \
               int x, int y, int *MedMas, int RH, int RW);
/* сравнение целых */
int comp(const void *i, const void *j)
{
    return *(int *)i - *(int *)j;
}
int filter()
{
    BITMAPFILEHEADER header __attribute__((unused));
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD **rgbMas = NULL;
    string FileIn = "file.bmp";
    string FileOut = "fileOut.bmp";
    int RH=3,RW=3;

    readBMP(FileIn,header,bmiHeader,rgbMas);

    RGBQUAD **NewrgbMas = new RGBQUAD *[bmiHeader.biWidth];
    int **temp = new int*[bmiHeader.biWidth];
    for(int i=0;i<bmiHeader.biWidth;i++)
    {
        temp[i] = new int[bmiHeader.biHeight];
        NewrgbMas[i] = new RGBQUAD[bmiHeader.biHeight];
    }

    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0;j<bmiHeader.biHeight;j++)
        {
            temp[i][j] = rgbMas[i][j].rgbRed;
        }
    medianfilter(bmiHeader, temp, RH, RW);
    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0;j<bmiHeader.biHeight;j++)
        {
            NewrgbMas[i][j].rgbRed = temp[i][j];
            NewrgbMas[i][j].rgbGreen = rgbMas[i][j].rgbGreen;
            NewrgbMas[i][j].rgbBlue = rgbMas[i][j].rgbBlue;
            NewrgbMas[i][j].rgbReserved = rgbMas[i][j].rgbReserved;
        }

    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0;j<bmiHeader.biHeight;j++)
        {
            temp[i][j] = rgbMas[i][j].rgbGreen;
        }
    medianfilter(bmiHeader, temp, RH, RW);
    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0;j<bmiHeader.biHeight;j++)
        {
            NewrgbMas[i][j].rgbGreen = temp[i][j];
        }

    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0;j<bmiHeader.biHeight;j++)
        {
            temp[i][j] = rgbMas[i][j].rgbReserved;
        }
    medianfilter(bmiHeader, temp, RH, RW);
    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0;j<bmiHeader.biHeight;j++)
        {
            NewrgbMas[i][j].rgbReserved = temp[i][j];
        }

    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0;j<bmiHeader.biHeight;j++)
        {
            temp[i][j] = rgbMas[i][j].rgbBlue;
        }
    medianfilter(bmiHeader, temp, RH, RW);
    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0;j<bmiHeader.biHeight;j++)
        {
            NewrgbMas[i][j].rgbBlue = temp[i][j];
        }

    writeBMP(FileOut,header,bmiHeader,NewrgbMas);

    for(int i=0; i<bmiHeader.biWidth; i++)
    {
        delete[] rgbMas[i];
        delete[] NewrgbMas[i];
        delete[] temp[i];
    }
    delete[] rgbMas;
    delete[] NewrgbMas;
    delete[] temp;

    return 0;
}


void readBMP(string fileName,BITMAPFILEHEADER& header, \
             BITMAPINFOHEADER& bmiHeader, RGBQUAD **&rgb)
{
    char* fn = new char[fileName.length()+1];
    strcpy(fn,fileName.c_str());
    FILE * pFile = fopen(fn, "rb");
    delete[] fn;
    // считываем заголовок файла
    header.bfType      = read_u16(pFile);
    header.bfSize      = read_u32(pFile);
    header.bfReserved1 = read_u16(pFile);
    header.bfReserved2 = read_u16(pFile);
    header.bfOffBits   = read_u32(pFile);

    // считываем заголовок изображения
    bmiHeader.biSize          = read_u32(pFile);
    bmiHeader.biWidth         = read_s32(pFile);
    bmiHeader.biHeight        = read_s32(pFile);
    bmiHeader.biPlanes        = read_u16(pFile);
    bmiHeader.biBitCount      = read_u16(pFile);
    bmiHeader.biCompression   = read_u32(pFile);
    bmiHeader.biSizeImage     = read_u32(pFile);
    bmiHeader.biXPelsPerMeter = read_s32(pFile);
    bmiHeader.biYPelsPerMeter = read_s32(pFile);
    bmiHeader.biClrUsed       = read_u32(pFile);
    bmiHeader.biClrImportant  = read_u32(pFile);


    rgb = new RGBQUAD*[bmiHeader.biWidth];
    for (int i = 0; i < bmiHeader.biWidth; i++)
    {
        rgb[i] = new RGBQUAD[bmiHeader.biHeight];
    }

    for (int i = 0; i < bmiHeader.biWidth; i++)
    {
        for (int j = 0; j < bmiHeader.biHeight; j++)
        {
            rgb[i][j].rgbRed = getc(pFile);
            rgb[i][j].rgbGreen = getc(pFile);
            rgb[i][j].rgbBlue = getc(pFile);
            rgb[i][j].rgbReserved = getc(pFile);
        }

        // пропускаем последний байт в строке
        //getc(pFile);
    }

    // выводим результат
    /*for (int i = 0; i < bmiHeader.biWidth; i++)
    {
        for (int j = 0; j < bmiHeader.biHeight; j++)
        {
            printf("%d %d %d\n", rgb[i][j].rgbRed, rgb[i][j].rgbGreen, rgb[i][j].rgbBlue);
        }
        printf("\n");
    }*/
    fclose(pFile);
}

void writeBMP(string fileName, BITMAPFILEHEADER header, \
              BITMAPINFOHEADER bmiHeader, RGBQUAD **rgb)
{
    char* fn = new char[fileName.length()+1];
    strcpy(fn,fileName.c_str());
    FILE * OutpFile = fopen(fn, "wb");
    delete[] fn;

    write_u16(header.bfType, OutpFile);
    write_u32(header.bfSize, OutpFile);
    write_u16(header.bfReserved1, OutpFile);
    write_u16(header.bfReserved2, OutpFile);
    write_u32(header.bfOffBits, OutpFile);

    write_u32(bmiHeader.biSize , OutpFile);
    write_s32(bmiHeader.biWidth, OutpFile);
    write_s32(bmiHeader.biHeight, OutpFile);
    write_u16(bmiHeader.biPlanes, OutpFile);
    write_u16(bmiHeader.biBitCount, OutpFile);
    write_u32(bmiHeader.biCompression, OutpFile);
    write_u32(bmiHeader.biSizeImage, OutpFile);
    write_s32(bmiHeader.biXPelsPerMeter, OutpFile);
    write_s32(bmiHeader.biYPelsPerMeter, OutpFile);
    write_u32(bmiHeader.biClrUsed, OutpFile);
    write_u32(bmiHeader.biClrImportant,OutpFile);

    for (int i = 0; i < bmiHeader.biWidth; i++)
    {
        for (int j = 0; j < bmiHeader.biHeight; j++)
        {
            putc(rgb[i][j].rgbRed,OutpFile);
            putc(rgb[i][j].rgbGreen,OutpFile);
            putc(rgb[i][j].rgbBlue,OutpFile);
            putc(rgb[i][j].rgbReserved,OutpFile);
        }
        //putc((char)NULL,OutpFile);
        //putc(10,OutpFile); //("\n",OutpFile);
        // пропускаем последний байт в строке
        //getc(pFile);
    }
    fclose(OutpFile);
}

void medianfilter(BITMAPINFOHEADER bmiHeader, int **&temp, int RH, int RW)
{
    int **NewTemp = new int*[bmiHeader.biWidth];
    for(int i=0;i<bmiHeader.biWidth;i++)
    {
        NewTemp[i] = new int[bmiHeader.biHeight];
    }
    int size = (2*RW+1)*(RH*2+1);
    for(int y = 0; y<bmiHeader.biWidth;y++)
    {
        for(int x=0; x<bmiHeader.biHeight;x++)
        {
            int* MedMas = new int[size];
            ZapMedMas(bmiHeader,temp,x,y,MedMas,RH,RW);
            qsort(MedMas, size, sizeof(int), comp);

            NewTemp[y][x] = MedMas[size/2];

            delete[] MedMas;
        }
    }
    for(int i=0;i<bmiHeader.biWidth;i++)
        for(int j=0; j<bmiHeader.biHeight;j++)
            temp[i][j] = NewTemp[j][i];

    for(int i=0; i<bmiHeader.biWidth; i++)
    {
        delete[] NewTemp[i];
    }
    delete[] NewTemp;
}


void ZapMedMas(BITMAPINFOHEADER bmiHeader, int **temp,\
               int x, int y, int *MedMas, int RH, int RW)
{
    int masind = 0;
    for(int dy = -RH; dy<RH;dy++)
    {
        for(int dx=-RW; dx<RW;dx++)
        {
            int ky,kx;
            if(y+dy<0)
                ky=0;
            else if (y+dy >= bmiHeader.biWidth)
                ky = bmiHeader.biWidth-1;
            else
                ky = y+dy;

            if(x+dx<0)
                kx=0;
            else if (x+dx >= bmiHeader.biHeight)
                kx = bmiHeader.biHeight-1;
            else
                kx = x+dx;

            MedMas[masind] = temp[kx][ky];
            ++masind;
        }
    }
}
