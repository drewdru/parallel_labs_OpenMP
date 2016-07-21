#include <iostream>
#include <omp.h>
#include <fstream>
#include <iomanip>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*#include <vector>

#include "Image_reader.h"

using namespace std;

void readfile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&);
void writefile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&);
double medianFilter (RGBQUAD **&, RGBQUAD **&, BITMAPINFOHEADER &, int, int);
void medMasFill(RGBQUAD **temp, int *medmasR, int *medmasG, int *medmasB, \
                int y, int x, int RH, int RW, int height, int width);
double medianFilterParallelFor(RGBQUAD** &rgb, RGBQUAD** &newrgb, \
                    BITMAPINFOHEADER &bmiHeader, int RH, int RW);
void QSort(int*, int);
int SecondFunc()
{

    RGBQUAD **rgb, **temp;//исходное, результат
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER bmiHeader;
    int RH=9,RW=9;

    const char* fname = "outFilter.txt";
    ofstream SaveOut(fname,ios::out|ios::app);
    /*for(int i =0;i<3;i++)
    {*/
       /* double sum = 0;
        readfile(rgb, header, bmiHeader);
        //for(int i=0;i<5;i++)
            sum += medianFilter(rgb, temp, bmiHeader, RH, RW);
        writefile(temp, header, bmiHeader);
        //cout<<sum/5<<endl;
        SaveOut<<sum<<endl;

        sum = 0;
        readfile(rgb, header, bmiHeader);
        //for(int i=0;i<5;i++)
            sum+=medianFilterParallelFor(rgb, temp, bmiHeader, RH, RW);
        writefile(temp, header, bmiHeader);
        //cout<<sum/5<<endl;
        SaveOut<<sum<<endl<<endl;
        /*RH+=3,RW+=3;
    }*/
 /*   SaveOut<<endl<<endl;
    SaveOut.close();

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        delete []rgb[i];
        delete []temp[i];
    }

    return 0;
}

void readfile(RGBQUAD** &rgb, BITMAPFILEHEADER &header,\
              BITMAPINFOHEADER &bmiHeader)
{
    FILE * pFile = fopen("file.bmp", "rb");

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


    rgb = new RGBQUAD*[bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        rgb[i] = new RGBQUAD[bmiHeader.biWidth];
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            rgb[i][j].rgbRed = getc(pFile);
            rgb[i][j].rgbGreen = getc(pFile);
            rgb[i][j].rgbBlue = getc(pFile);
        }

    }


    fclose(pFile);
}

void writefile(RGBQUAD** &rgb, BITMAPFILEHEADER &header, \
               BITMAPINFOHEADER &bmiHeader)
{
    FILE * oFile = fopen("out.bmp", "wb");

    write_u16(header.bfType, oFile);
    write_u32(header.bfSize, oFile);
    write_u16(header.bfReserved1, oFile);
    write_u16(header.bfReserved2, oFile);
    write_u32(header.bfOffBits, oFile);

    write_u32(bmiHeader.biSize , oFile);
    write_s32(bmiHeader.biWidth, oFile);
    write_s32(bmiHeader.biHeight, oFile);
    write_u16(bmiHeader.biPlanes, oFile);
    write_u16(bmiHeader.biBitCount, oFile);
    write_u32(bmiHeader.biCompression, oFile);
    write_u32(bmiHeader.biSizeImage, oFile);
    write_s32(bmiHeader.biXPelsPerMeter, oFile);
    write_s32(bmiHeader.biYPelsPerMeter, oFile);
    write_u32(bmiHeader.biClrUsed, oFile);
    write_u32(bmiHeader.biClrImportant,oFile);

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            putc(rgb[i][j].rgbRed,oFile);
            putc(rgb[i][j].rgbGreen,oFile);
            putc(rgb[i][j].rgbBlue,oFile);
        }

    }
    fclose(oFile);
}

double medianFilter(RGBQUAD** &rgb, RGBQUAD** &newrgb, \
                    BITMAPINFOHEADER &bmiHeader, int RH, int RW)
{
    int i, j;
    //newrgb - массив-результат
    newrgb = new RGBQUAD*[bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        newrgb[i] = new RGBQUAD[bmiHeader.biWidth];
    }


    //размер апертуры фильтра
    int filterSize = (RH*2+1)*(RW*2+1);

    double st = omp_get_wtime();
    for(i = 0; i < bmiHeader.biHeight; i++)
        for(j = 0; j < bmiHeader.biWidth; j++)
        {
            int * medmasR = new int [filterSize];
            int * medmasG = new int [filterSize];
            int * medmasB = new int [filterSize];

            medMasFill (rgb, medmasR,medmasG, medmasB, i, j, RH, RW, \
                        bmiHeader.biHeight, bmiHeader.biWidth);
            QSort(medmasR, filterSize);
            QSort(medmasG, filterSize);
            QSort(medmasB, filterSize);
            newrgb[i][j].rgbRed = medmasR[filterSize/2];
            newrgb[i][j].rgbGreen = medmasG[filterSize/2];
            newrgb[i][j].rgbBlue = medmasB[filterSize/2];
            delete []medmasR;
            delete []medmasG;
            delete []medmasB;
        }
    st = omp_get_wtime() - st;
    return st;
}

void medMasFill(RGBQUAD **temp, int *medmasR, int *medmasG, int *medmasB, \
                int y, int x, int RH, int RW, int height, int width)
{
    int masmind = 0, kx, ky;
    for(int dy = -RH; dy <= RH; dy++)
        for(int dx = -RW; dx <= RW; dx++)
        {
            if (y + dy < 0)
                ky = 0;
            else if (y + dy > height-1)
                ky = height - 1;
            else
                ky = y + dy;

            if (x + dx < 0)
                kx = 0;
            else if (x + dx  > width-1)
                kx = width - 1;
            else
                kx = x + dx;
            medmasR[masmind] = temp[ky][kx].rgbRed;
            medmasG[masmind] = temp[ky][kx].rgbGreen;
            medmasB[masmind++] = temp[ky][kx].rgbBlue;
        }
}

void QSort(int* a, int count)
{
    const int MAXSTACK = 2048;
    long i, j;
    long lb, ub;
    long lbstack[MAXSTACK], ubstack[MAXSTACK];
    long stackpos = 1, ppos;
    int pivot, temp;

    lbstack[1] = 0;
    ubstack[1] = count-1;

    do
    {
        lb = lbstack[ stackpos ];
        ub = ubstack[ stackpos ];
        stackpos--;

        do
        {
            ppos = ( lb + ub ) >> 1;
            i = lb; j = ub; pivot = a[ppos];

            do
            {
                while ( a[i] < pivot ) i++;
                while ( pivot < a[j] ) j--;

                if ( i <= j )
                {
                    temp = a[i]; a[i] = a[j]; a[j] = temp;
                    i++; j--;
                }
            }
            while ( i <= j );

            if ( i < ppos )
            {
                if ( i < ub )
                {
                    stackpos++;
                    lbstack[ stackpos ] = i;
                    ubstack[ stackpos ] = ub;
                }
                ub = j;

            }
            else
            {
                if ( j > lb )
                {
                    stackpos++;
                    lbstack[ stackpos ] = lb;
                    ubstack[ stackpos ] = j;
                }
                lb = i;
            }

        }
        while ( lb < ub );
    }
    while ( stackpos != 0 );
}

double medianFilterParallelFor(RGBQUAD** &rgb, RGBQUAD** &newrgb, \
                    BITMAPINFOHEADER &bmiHeader, int RH, int RW)
{
    //newrgb - массив-результат
    newrgb = new RGBQUAD*[bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        newrgb[i] = new RGBQUAD[bmiHeader.biWidth];
    }


    //размер апертуры фильтра
    int filterSize = (RH*2+1)*(RW*2+1);
    int i = 0;
    int j = 0;
    double st = omp_get_wtime();
#pragma omp parallel for shared(newrgb)
    for(i = 0; i < bmiHeader.biHeight; i++)
#pragma omp parallel for shared(newrgb)
        for(j = 0; j < bmiHeader.biWidth; j++)
        {
            int * medmasR = new int [filterSize];
            int * medmasG = new int [filterSize];
            int * medmasB = new int [filterSize];

            medMasFill (rgb, medmasR,medmasG, medmasB, i, j, RH, RW, \
                        bmiHeader.biHeight, bmiHeader.biWidth);

            QSort(medmasR, filterSize);
            QSort(medmasG, filterSize);
            QSort(medmasB, filterSize);

            newrgb[i][j].rgbRed = medmasR[filterSize/2];
            newrgb[i][j].rgbGreen = medmasG[filterSize/2];
            newrgb[i][j].rgbBlue = medmasB[filterSize/2];
            delete []medmasR;
            delete []medmasG;
            delete []medmasB;
        }
    st = omp_get_wtime() - st;
    return st;
}
*/
