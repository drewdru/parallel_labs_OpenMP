#include <iostream>
#include <omp.h>
#include <fstream>
#include <string>
#include "Image_reader.h"
#include <math.h>

//#define out cout
using namespace std;

void readfile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&, int );
void writefile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&, string, int );
double omp_mom1(RGBQUAD **&, RGBQUAD **&, BITMAPINFOHEADER &, int, double, double, int);

int getY (RGBQUAD p)//яркость
{
    return (int)0.299*p.rgbRed+0.587*p.rgbBlue+0.114*p.rgbBlue;
}

int analys()
{
    RGBQUAD **rgb, **temp;//исходное, результат
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER bmiHeader;

    string features[18] = { "Moment1_omp", "Moment2_omp", "Moment3_omp", "Moment4_omp", "Moment5_omp", "Moment6_omp", "Smoothness_omp", "Uniformity_omp", "Entropy_omp"
                    "Moment1_cilk", "Moment2_cilk", "Moment3_cilk", "Moment4_cilk", "Moment5_cilk", "Moment6_cilk", "Smoothness_cilk", "Uniformity_cilk", "Entropy_cilk"};
    double(*func[1])(RGBQUAD **&, RGBQUAD **&, BITMAPINFOHEADER &, int , double , double, int ) = { omp_mom1/*, omp_mom2, omp_mom3, omp_mom4, omp_mom5, omp_mom6, omp_smooth, omp_uni, omp_ent
    */ };
    omp_set_num_threads(4);
    readfile(rgb, header, bmiHeader, 0);
    cout<<"OMP. Гладкость"<<": "<<omp_mom1(rgb, temp, bmiHeader, 5, 0.33, 0.66, 4)<<"\n";
    writefile(temp, header, bmiHeader, "texture/glad_omp_5.bmp" , 0);

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        delete []temp[i];
    }
    delete []temp;
    return 0;

}

double omp_mom1(RGBQUAD **&rgb, RGBQUAD **&newrgb, BITMAPINFOHEADER &bmiHeader, int radius, double T1, double T2, int thr)
{

    newrgb = new RGBQUAD*[bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        newrgb[i] = new RGBQUAD[bmiHeader.biWidth];
    }


    double hysto[256];
    double t = omp_get_wtime();
    int j=0, k=0;
    RGBQUAD pixel;
    pixel.rgbRed = 0;
    pixel.rgbGreen = 0;
    pixel.rgbBlue = 0;
    pixel.rgbReserved = 0;
    double summ = 0;
    int l =0;
    int index=0;
    double moment = 0;
    int i = 0;
#pragma omp parallel for shared(newrgb, rgb)\
private(i,j,k,pixel,summ,l,index,moment,hysto)
    for(i = 0; i < bmiHeader.biHeight; i++)
    {
        for(j = 0; j < bmiHeader.biWidth; j++)
        {
            for (k = 0; k < 256; k++)
            {hysto[k] = 0;}

            pixel.rgbRed = 0;
            pixel.rgbGreen = 0;
            pixel.rgbBlue = 0;
            pixel.rgbReserved = 0;

            summ = 0;//сумма яркостей пикселей
            //получение гистограммы
            for (k = -radius; k <= radius; k++)
            {
                l = j + k;
                if (l < 0 || l > bmiHeader.biWidth-1)
                    continue;
                index = getY(rgb[i][j]);
                summ+=index;
                hysto[index]++;
            }

            moment = 0;
            for (k = 0; k < 256; k++)
            {
                moment += pow(hysto[k]-summ/256, 3)*(hysto[k]/summ);
            }

            if (moment>0 && moment<T1)
            {
                pixel.rgbGreen = 255;
            }
            else if (moment>T1 && moment<T2)
            {
                pixel.rgbRed = 255;
                pixel.rgbGreen = 255;

                //pixel.rgbBlue = 255;
            }
            else if (moment>T2)
            {
                pixel.rgbRed = 255;
            }
            else
                pixel.rgbRed = 255;
            newrgb[i][j].rgbRed = pixel.rgbBlue;
            newrgb[i][j].rgbGreen= pixel.rgbGreen;
            newrgb[i][j].rgbBlue = pixel.rgbRed;
        }
    }
    return omp_get_wtime()-t;
}

void readfile(RGBQUAD** &rgb, BITMAPFILEHEADER &header, BITMAPINFOHEADER &bmiHeader, int q)
{
    FILE * pFile = fopen("texture/input.bmp", "rb");

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

void writefile(RGBQUAD** &rgb, BITMAPFILEHEADER &header, BITMAPINFOHEADER &bmiHeader, string name, int q)
{
    const char * n = name.c_str();

    FILE * oFile = fopen(n, "wb");

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
