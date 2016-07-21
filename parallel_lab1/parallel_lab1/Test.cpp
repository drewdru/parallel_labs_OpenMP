#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iomanip>

using namespace std;

void Test()//Задание №3.2
{
    int n = 18; int m = 3;
    double** mas = new double*[n];
    double** masSp = new double*[n];
    double** masEp = new double*[n];
    double** masCp = new double*[n];
    for(int i = 0; i<n; i++)
    {
        mas[i] = new double[m];
        masSp[i] = new double[m];
        masEp[i] = new double[m];
        masCp[i] = new double[m];
    }

    ifstream in("in.txt",ios::in|ios::app);
    for(int i = 0; i<n; i++)
        for(int j=0; j<m; j++)
            in>>mas[i][j];
    in.close();

    //считывает время работы программы
    for(int i = 0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            in>>mas[i][j];
        }
    }

    //ускорение
    masSp[0][0] = mas[0][0]/mas[0][0];
    masSp[0][1] = mas[0][1]/mas[0][0];
    masSp[0][2] = mas[0][2]/mas[0][0];
    for(int i = 1; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            masSp[i][j] =  mas[0][0]/mas[i][j];
        }
    }

    //Эффективность
    for(int i = 0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            masEp[i][j] =  masSp[i][j]/(j+2);
        }
    }

    //Стоимость
    for(int i = 0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            masCp[i][j] =  (j+2)*mas[i][j];
        }
    }

    ofstream out1 ("outSP.txt",ios::out|ios::trunc);
    for(int i = 0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            out1<<masSp[i][j]<<"\t";
        }
        out1<<endl;
    }
    out1.close();

    ofstream out2 ("outEp.txt",ios::out|ios::trunc);
    for(int i = 0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            out2<<masEp[i][j]<<"\t";
        }
        out2<<endl;
    }
    out2.close();

    ofstream out3 ("outCp.txt",ios::out|ios::trunc);
    for(int i = 0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            out3<<masCp[i][j]<<"\t";
        }
        out3<<endl;
    }
    out3.close();


    for (int count = 0; count < n; count++)
    {
        delete [] mas[count];
        delete [] masSp[count];
        delete [] masEp[count];
        delete [] masCp[count];
    }
}
