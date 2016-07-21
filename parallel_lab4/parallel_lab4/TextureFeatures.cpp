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
void writefile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&, \
                const char *fout);
void readfile (RGBQUAD **&, BITMAPFILEHEADER&, BITMAPINFOHEADER&);


double mu(int Size,RGBQUAD**& image,\
                          BITMAPINFOHEADER &bmiHeader);
void TextureFeatures()
{
    RGBQUAD **rgb;//, **temp;//исходное, результат
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER bmiHeader;
    char* fname = "TextureFeatures.bmp";

    //m =  от 1 до 6
    int m = 5;
    readfile(rgb, header, bmiHeader);
    double time = mu(m,rgb, bmiHeader);
    writefile(rgb, header, bmiHeader, fname);

}

double mu(int Size, RGBQUAD**& image, \
                          BITMAPINFOHEADER &bmiHeader)
{
    int bmisize = bmiHeader.biHeight*bmiHeader.biWidth;
    RGBQUAD* temp = new RGBQUAD[bmisize];

    int cnt = 0;

    for (int i = 0; i < bmiHeader.biHeight; i++)
    {
        for (int j = 0; j < bmiHeader.biWidth; j++)
        {
            temp[cnt++].rgbRed = image[i][j].rgbRed;
            temp[cnt++].rgbGreen = image[i][j].rgbGreen;
            temp[cnt++].rgbBlue = image[i][j].rgbBlue;
        }
    }

    double time = omp_get_wtime();

    double KoefMuRed = 0;
    double KoefMuGreen = 0;
    double KoefMuBlue = 0;

    for(int i = 0; i <bmisize*3;i++)
    {

    }

    delete []temp;
    return omp_get_wtime() - time;
}
