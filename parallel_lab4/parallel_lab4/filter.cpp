#include <iostream>
#include <omp.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "Image_reader.h"

using namespace std;

//void readfile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&);
void writefile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&, const char *fout);
void readfile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&);

double CountGauss(double r, int filterSize, RGBQUAD**& image, \
                  BITMAPINFOHEADER &bmiHeader);
double meanFilter(double r, int filterSize, RGBQUAD**& image,\
                  BITMAPINFOHEADER &bmiHeader);
double ParallGauss(double r, int filterSize, RGBQUAD**& image, \
                   BITMAPINFOHEADER &bmiHeader);
double ParallelmeanFilter(double r, int filterSize, RGBQUAD**& image,\
                          BITMAPINFOHEADER &bmiHeader);
int SecondFunc()
{
    RGBQUAD **rgb;//, **temp;//исходное, результат
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER bmiHeader;
    string OutName;
    char* fname = "out.txt";
    ofstream SaveOut(fname,ios::out|ios::app);
    double r = 0.5;
    int filterSize;
    double sum=0;

    for(int j=9;j<=20;j+=3)
    {
        filterSize = j;
        stringstream ss1; ss1<<filterSize;
        sum=0;
        readfile(rgb, header, bmiHeader);
        for(int i =0;i<5;i++)
            sum+=meanFilter(r, filterSize, rgb, bmiHeader);
        OutName = "meanFilter_size=";
        OutName+= ss1.str()+".bmp\0";
        writefile(rgb, header, bmiHeader, OutName.c_str());
        cout<<sum/5<<endl;
        SaveOut<<sum/5<<endl;

        sum =0;
        readfile(rgb, header, bmiHeader);
        for(int i =0;i<5;i++)
            sum+=ParallelmeanFilter(r, filterSize, rgb, bmiHeader);
        OutName = "PARALmeanFilter_size=";
        OutName+= ss1.str()+".bmp\0";
        writefile(rgb, header, bmiHeader, OutName.c_str());
        cout<<sum/5<<endl;
        SaveOut<<sum/5<<endl;

        sum =0;
        readfile(rgb, header, bmiHeader);
        for(int i =0;i<5;i++)
            sum+=CountGauss(r, filterSize, rgb, bmiHeader);
        OutName = "Gaussfilter_size=";
        OutName+= ss1.str()+".bmp\0";
        writefile(rgb, header, bmiHeader, OutName.c_str());
        cout<<sum/5<<endl;
        SaveOut<<sum/5<<endl;

        sum =0;
        readfile(rgb, header, bmiHeader);
        for(int i =0;i<5;i++)
            sum+=ParallGauss(r, filterSize, rgb, bmiHeader);
        OutName = "PARLGaussfilter_size=";
        OutName+= ss1.str()+".bmp\0";
        writefile(rgb, header, bmiHeader, OutName.c_str());
        cout<<sum/5<<endl;
        SaveOut<<sum/5<<endl;



        SaveOut<<endl;
    }

    SaveOut<<endl<<endl;
    SaveOut.close();

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        delete []rgb[i];
        //delete []temp[i];
    }
    delete []rgb;
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
               BITMAPINFOHEADER &bmiHeader,const char* fout)
{
    //char* fout = "fout.bmp\0";
    FILE * oFile = fopen(fout, "wb");

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

double CountGauss(double r, int filterSize, RGBQUAD**& image,\
                  BITMAPINFOHEADER &bmiHeader)
{
    RGBQUAD** temp = new RGBQUAD*[bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        temp[i] = new RGBQUAD[bmiHeader.biWidth];
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            temp[i][j].rgbRed = image[i][j].rgbRed;
            temp[i][j].rgbGreen = image[i][j].rgbGreen;
            temp[i][j].rgbBlue = image[i][j].rgbBlue;
        }
    }

    double time = omp_get_wtime();
    if (r > 0)
    {
        double** blurArray = new double*[filterSize];
        for (int i = 0; i < filterSize; i++)
        {
            blurArray[i] = new double[filterSize];
        }
        double div = 0;
        int pixelPosX;
        int pixelPosY;

        for (int i = 0; i < filterSize; i++)
        {
            for (int j = 0; j < filterSize; j++)
            {
                blurArray[i][j] = (double)(1 / (M_PI * 2 * r * r)) * \
                        exp((fabs((filterSize / 2) - i) *
                             fabs((filterSize / 2) - i)) * \
                            (-1.0) / (2 * r * r));
                div += blurArray[i][j];
            }
        }

        for (int y = 0; y < bmiHeader.biHeight;/* - \
                  filterSize / 2;*/ y++)
        {
            for (int x = 0; x < bmiHeader.biWidth;/* -\
                      filterSize / 2;*/ x++)
            {
                double rSum = 0, gSum = 0, bSum = 0, kSum = 0;

                for (int i = 0; i < filterSize; i++)
                {
                    for (int j = 0; j < filterSize; j++)
                    {
                        pixelPosX = x + j - filterSize / 2;
                        pixelPosY = y + i - filterSize / 2;
                        if(pixelPosX<0)
                            pixelPosX+=filterSize / 2;
                        if(pixelPosY<0)
                            pixelPosY+=filterSize / 2;

                        if(pixelPosX>bmiHeader.biWidth)
                            continue;
                        if(pixelPosY>bmiHeader.biHeight)
                            continue;
                        if(pixelPosX==bmiHeader.biWidth)
                            pixelPosX=bmiHeader.biWidth - (filterSize / 2)-1;
                        if(pixelPosY==bmiHeader.biHeight)
                            pixelPosY=bmiHeader.biHeight-(filterSize / 2)-1;

                        int red = temp[pixelPosY][pixelPosX].rgbRed;
                        int green = temp[pixelPosY][pixelPosX].rgbGreen;
                        int blue = temp[pixelPosY][pixelPosX].rgbBlue;

                        double kernelVal = blurArray[i][j];

                        rSum += red * kernelVal;
                        gSum += green * kernelVal;
                        bSum += blue * kernelVal;

                        kSum += kernelVal;
                    }
                }

                if (kSum <= 0) kSum = 1;

                rSum /= kSum;
                if (rSum < 0) rSum = 0;
                if (rSum > 255) rSum = 255;

                gSum /= kSum;
                if (gSum < 0) gSum = 0;
                if (gSum > 255) gSum = 255;

                bSum /= kSum;
                if (bSum < 0) bSum = 0;
                if (bSum > 255) bSum = 255;

                temp[y][x].rgbRed = rSum;
                temp[y][x].rgbGreen = gSum;
                temp[y][x].rgbBlue = bSum;
            }
        }
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            image[i][j].rgbRed = temp[i][j].rgbRed;
            image[i][j].rgbGreen = temp[i][j].rgbGreen;
            image[i][j].rgbBlue = temp[i][j].rgbBlue;
        }
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
        {
            delete []temp[i];
        }
    delete []temp;

    return omp_get_wtime() - time;
}

double ParallGauss(double r, int filterSize, RGBQUAD**& image,\
                   BITMAPINFOHEADER &bmiHeader)
{
    RGBQUAD** temp = new RGBQUAD*[bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        temp[i] = new RGBQUAD[bmiHeader.biWidth];
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            temp[i][j].rgbRed = image[i][j].rgbRed;
            temp[i][j].rgbGreen = image[i][j].rgbGreen;
            temp[i][j].rgbBlue = image[i][j].rgbBlue;
        }
    }

    double time = omp_get_wtime();
    if (r > 0)
    {
        double** blurArray = new double*[filterSize];
        for (int i = 0; i < filterSize; i++)
        {
            blurArray[i] = new double[filterSize];
        }
        double div = 0;
        int pixelPosX;
        int pixelPosY;
        int i = 0; int j = 0;
        for ( i= 0; i < filterSize; i++)
        {
#pragma omp parallel for shared(blurArray)
            for ( j= 0; j < filterSize; j++)
            {
                blurArray[i][j] = (double)(1 / (M_PI * 2 * r * r)) * \
                        exp((fabs((filterSize / 2) - i) *
                             fabs((filterSize / 2) - i)) * \
                            (-1.0) / (2 * r * r));
                div += blurArray[i][j];
            }
        }
        int y = 0;int x = 0;int red;int green;int blue;double kernelVal;
#pragma omp parallel for shared(temp,blurArray) \
    private(pixelPosX,pixelPosY,i,j,x,y,red,green,blue,kernelVal)
        for (y = 0; y < bmiHeader.biHeight; y++)
        {
            for (x = 0; x < bmiHeader.biWidth; x++)
            {
                double rSum = 0, gSum = 0, bSum = 0, kSum = 0;
                /*#pragma omp parallel for \
    private(pixelPosX,pixelPosY,i,j,red,green,blue,kernelVal)\
    reduction(+:rSum,gSum,bSum,kSum)*/
                for (i = 0; i < filterSize; i++)
                {
                    for (j = 0; j < filterSize; j++)
                    {
                        pixelPosX = x + j - filterSize / 2;
                        pixelPosY = y + i - filterSize / 2;
                        if(pixelPosX<0)
                            pixelPosX+=filterSize / 2;
                        if(pixelPosY<0)
                            pixelPosY+=filterSize / 2;

                        if(pixelPosX>bmiHeader.biWidth)
                            continue;
                        if(pixelPosY>bmiHeader.biHeight)
                            continue;
                        if(pixelPosX==bmiHeader.biWidth)
                            pixelPosX=bmiHeader.biWidth - (filterSize / 2)-1;
                        if(pixelPosY==bmiHeader.biHeight)
                            pixelPosY=bmiHeader.biHeight-(filterSize / 2)-1;

                        red = temp[pixelPosY][pixelPosX].rgbRed;
                        green = temp[pixelPosY][pixelPosX].rgbGreen;
                        blue = temp[pixelPosY][pixelPosX].rgbBlue;

                        kernelVal = blurArray[i][j];

                        rSum += red * kernelVal;
                        gSum += green * kernelVal;
                        bSum += blue * kernelVal;

                        kSum += kernelVal;
                    }
                }

                if (kSum <= 0) kSum = 1;

                rSum /= kSum;
                if (rSum < 0) rSum = 0;
                if (rSum > 255) rSum = 255;

                gSum /= kSum;
                if (gSum < 0) gSum = 0;
                if (gSum > 255) gSum = 255;

                bSum /= kSum;
                if (bSum < 0) bSum = 0;
                if (bSum > 255) bSum = 255;

                temp[y][x].rgbRed = rSum;
                temp[y][x].rgbGreen = gSum;
                temp[y][x].rgbBlue = bSum;
            }
        }
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            image[i][j].rgbRed = temp[i][j].rgbRed;
            image[i][j].rgbGreen = temp[i][j].rgbGreen;
            image[i][j].rgbBlue = temp[i][j].rgbBlue;
        }
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        delete []temp[i];
    }
    delete []temp;
    return omp_get_wtime() - time;
}

double meanFilter(double r, int filterSize, RGBQUAD**& image,\
                  BITMAPINFOHEADER &bmiHeader)
{
    RGBQUAD** temp = new RGBQUAD*[bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        temp[i] = new RGBQUAD[bmiHeader.biWidth];
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            temp[i][j].rgbRed = image[i][j].rgbRed;
            temp[i][j].rgbGreen = image[i][j].rgbGreen;
            temp[i][j].rgbBlue = image[i][j].rgbBlue;
        }
    }

    double time = omp_get_wtime();
    if (r > 0)
    {
        int pixelPosX=0;
        int pixelPosY=0;

        for (int y = 0; y < bmiHeader.biHeight;y++)
        {
            for (int x = 0; x < bmiHeader.biWidth;x++)
            {
                double rSum = 0, gSum = 0, bSum = 0, kSum = 0;

                for (int i = 0; i < filterSize; i++)
                {
                    for (int j = 0; j < filterSize; j++)
                    {
                        if((i==j)||(i+j==filterSize-1))
                        {
                            pixelPosX = x + j - filterSize / 2;
                            pixelPosY = y + i - filterSize / 2;
                            if(pixelPosX<0)
                                pixelPosX+=filterSize / 2;
                            if(pixelPosY<0)
                                pixelPosY+=filterSize / 2;

                            if(pixelPosX>=bmiHeader.biWidth)
                                pixelPosX=bmiHeader.biWidth-(filterSize / 2)-1;
                            if(pixelPosY>=bmiHeader.biHeight)
                                pixelPosY=bmiHeader.biHeight-(filterSize / 2)-1;

                            int red = temp[pixelPosY][pixelPosX].rgbRed;
                            int green = temp[pixelPosY][pixelPosX].rgbGreen;
                            int blue = temp[pixelPosY][pixelPosX].rgbBlue;

                            rSum += red;
                            gSum += green;
                            bSum += blue;

                            ++kSum;
                        }
                    }
                }

                if (kSum <= 0) kSum = 1;

                rSum /= kSum;
                if (rSum < 0) rSum = 0;
                if (rSum > 255) rSum = 255;

                gSum /= kSum;
                if (gSum < 0) gSum = 0;
                if (gSum > 255) gSum = 255;

                bSum /= kSum;
                if (bSum < 0) bSum = 0;
                if (bSum > 255) bSum = 255;

                temp[y][x].rgbRed = rSum;
                temp[y][x].rgbGreen = gSum;
                temp[y][x].rgbBlue = bSum;
            }
        }
    }
    int i = 0;int j = 0;
#pragma omp parallel for private(i,j) shared(image)
    for (i = 0; i < bmiHeader.biHeight; i++)
    {
        for (j = 0; j < bmiHeader.biWidth; j++)
        {
            image[i][j].rgbRed = temp[i][j].rgbRed;
            image[i][j].rgbGreen = temp[i][j].rgbGreen;
            image[i][j].rgbBlue = temp[i][j].rgbBlue;
        }
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        delete []temp[i];
    }
    delete []temp;
    return omp_get_wtime() - time;
}

double ParallelmeanFilter(double r, int filterSize, RGBQUAD**& image,\
                          BITMAPINFOHEADER &bmiHeader)
{
    RGBQUAD** temp = new RGBQUAD*[bmiHeader.biHeight];
    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        temp[i] = new RGBQUAD[bmiHeader.biWidth];
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            temp[i][j].rgbRed = image[i][j].rgbRed;
            temp[i][j].rgbGreen = image[i][j].rgbGreen;
            temp[i][j].rgbBlue = image[i][j].rgbBlue;
        }
    }

    double time = omp_get_wtime();
    if (r > 0)
    {
        int pixelPosX=0;
        int pixelPosY=0;
        int i = 0;int j = 0;
        int y = 0;int x = 0;int red;int green;int blue;
        double rSum = 0, gSum = 0, bSum = 0, kSum = 0;
#pragma omp parallel for shared(temp) \
    private(pixelPosX,pixelPosY,i,j,x,y,red,green,blue,rSum,gSum,bSum,kSum)
        for (y = 0; y < bmiHeader.biHeight;y++)
        {
            for (x = 0; x < bmiHeader.biWidth;x++)
            {
                /*double*/ rSum = 0, gSum = 0, bSum = 0, kSum = 0;

                for (i = 0; i < filterSize; i++)
                {
                    for (j = 0; j < filterSize; j++)
                    {
                        if((i==j)||(i+j==filterSize-1))
                        {
                            pixelPosX = x + j - filterSize / 2;
                            pixelPosY = y + i - filterSize / 2;
                            if(pixelPosX<0)
                                pixelPosX+=filterSize / 2;
                            if(pixelPosY<0)
                                pixelPosY+=filterSize / 2;
                            if(pixelPosX>=bmiHeader.biWidth)
                                pixelPosX=bmiHeader.biWidth-(filterSize / 2)-1;
                            if(pixelPosY>=bmiHeader.biHeight)
                                pixelPosY=bmiHeader.biHeight-(filterSize / 2)-1;

                            red = temp[pixelPosY][pixelPosX].rgbRed;
                            green = temp[pixelPosY][pixelPosX].rgbGreen;
                            blue = temp[pixelPosY][pixelPosX].rgbBlue;

                            rSum += red;
                            gSum += green;
                            bSum += blue;

                            ++kSum;
                        }
                    }
                }

                if (kSum <= 0) kSum = 1;

                rSum /= kSum;
                if (rSum < 0) rSum = 0;
                if (rSum > 255) rSum = 255;

                gSum /= kSum;
                if (gSum < 0) gSum = 0;
                if (gSum > 255) gSum = 255;

                bSum /= kSum;
                if (bSum < 0) bSum = 0;
                if (bSum > 255) bSum = 255;

                temp[y][x].rgbRed = rSum;
                temp[y][x].rgbGreen = gSum;
                temp[y][x].rgbBlue = bSum;
            }
        }
    }

    int i = 0;int j = 0;
#pragma omp parallel for private(i,j) shared(image)
    for (i = 0; i < bmiHeader.biHeight; i++)
    {
        for (j = 0; j < bmiHeader.biWidth; j++)
        {
            image[i][j].rgbRed = temp[i][j].rgbRed;
            image[i][j].rgbGreen = temp[i][j].rgbGreen;
            image[i][j].rgbBlue = temp[i][j].rgbBlue;
        }
    }

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        delete []temp[i];
    }
    delete []temp;
    return omp_get_wtime() - time;
}

