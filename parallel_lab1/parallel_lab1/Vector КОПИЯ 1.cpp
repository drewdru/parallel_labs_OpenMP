#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iomanip>

using namespace std;

void FillingMas(double** mas, int n);
void FillingMasParallel(double** mas, int n, int thread);

void SumAB(double** masA, double** masB,double** masC, int n);
void SumABParallelFor(double** masA, double** masB,double** masC, int n, int thread);
void SumABParallelSection(double** masA, double** masB, double** masC, int n, int thread);

double SumC(double** masC, int n);
double SumCCritical(double** masC, int n, int thread);
double SumCReductor(double** masC, int n, int thread);
void Vector()
{

    int MasLen = 5;
    double** masA = new double*[MasLen];
    double** masB = new double*[MasLen];
    double** masC = new double*[MasLen];

    for(int i = 0; i<MasLen;i++)
    {
        masA[i] = new double[MasLen];
        masB[i] = new double[MasLen];
        masC[i] = new double[MasLen];
    }
    double timeStart, timeEnd;
    char* fname = "out.txt";
    ofstream SaveOut(fname,ios::out|ios::trunc);

    timeStart = omp_get_wtime ();
    FillingMas(masA,MasLen);
    timeEnd = omp_get_wtime ();
    cout<<"Время заполнения матрицы = "<<(timeEnd - timeStart)<<endl;
    SaveOut<<(timeEnd - timeStart)<<endl;


    for (int i = 2; i<5; i++)
    {
        timeStart = omp_get_wtime ();
        FillingMasParallel(masB,MasLen,i);
        timeEnd = omp_get_wtime ();
        cout<<"Количество потоков"<<i<<": ";
        cout<<"Время паралельного заполнения матрицы = " \
           <<(timeEnd - timeStart)<<endl;
        SaveOut<<(timeEnd - timeStart)<<endl;
    }

    timeStart = omp_get_wtime ();
    SumAB(masA, masB, masC,MasLen);
    timeEnd = omp_get_wtime ();
    cout<<"Время сложения двух матрицы = " \
       <<(timeEnd - timeStart)<<endl;
    SaveOut<<(timeEnd - timeStart)<<endl;

    for (int i = 2; i<5; i++)
    {
        timeStart = omp_get_wtime ();
        SumABParallelFor(masA, masB, masC,MasLen,i);
        timeEnd = omp_get_wtime ();
        cout<<"Количество потоков"<<i<<": ";
        cout<<"Время сложения двух матрицы  параллельно " \
              "с использованием цикла FOR = " \
           <<(timeEnd - timeStart)<<endl;
        SaveOut<<(timeEnd - timeStart)<<endl;
    }

    for (int i = 2; i<5; i++)
    {
        timeStart = omp_get_wtime ();
        SumABParallelSection(masA, masB, masC,MasLen, i);
        timeEnd = omp_get_wtime ();
        cout<<"Количество потоков"<<i<<": ";
        cout<<"Время сложения двух матрицы  параллельно " \
              "с использованием секций = " \
           <<(timeEnd - timeStart)<<endl;
        SaveOut<<(timeEnd - timeStart)<<endl;
    }

    timeStart = omp_get_wtime ();
    SumC(masC,MasLen);
    timeEnd = omp_get_wtime ();
    cout<<"Время поиска суммы элементов матрицы = " \
       <<(timeEnd - timeStart)<<endl;
    SaveOut<<(timeEnd - timeStart)<<endl;

    for (int i = 2; i<5; i++)
    {
        timeStart = omp_get_wtime ();
        SumCCritical(masC,MasLen,i);
        timeEnd = omp_get_wtime ();
        cout<<"Количество потоков"<<i<<": ";
        cout<<"Время поиска суммы элементов матрицы " \
              "Параллельном с использованием критических секций = " \
           <<(timeEnd - timeStart)<<endl;
        SaveOut<<(timeEnd - timeStart)<<endl;
    }

    for (int i = 2; i<5; i++)
    {
        timeStart = omp_get_wtime ();
        SumCReductor(masC,MasLen,i);
        timeEnd = omp_get_wtime ();
        cout<<"Количество потоков"<<i<<": ";
        cout<<"Время поиска суммы элементов матрицы " \
              "Параллельном с использованием редукторов = " \
           <<(timeEnd - timeStart)<<endl;
        SaveOut<<(timeEnd - timeStart)<<endl;
    }

    SaveOut.close();

    for (int count = 0; count < MasLen; count++)
    {
        delete [] masA[count];
        delete [] masB[count];
        delete [] masC[count];
    }
}

void FillingMas(double** mas, int n)
{
    for(int i = 0; i<n; i++ )
    {
        for(int j = 0; j<n; j++)
        {
            double a = i+1;
            double b = 1/(j+1);
            mas[i][j] = pow(a,b) * cos(i+j);
        }
    }
}
void FillingMasParallel(double **mas, int n, int thread)
{
    omp_set_dynamic(0);      // Не менять число потоков во время исполнения
    omp_set_num_threads(thread);
    int i,j;
#pragma omp parallel shared(mas) private(i,j)
    {
        for(i = 0; i<n; i++ )
        {
            for(j = 0; j<n; j++)
            {
                double a = i+1;
                double b = 1/(j+1);
                mas[i][j] = pow(a,b) * cos(i+j);
            }
        }
    }
}

void SumAB(double** masA, double** masB,double** masC, int n)
{
    for(int i = 0; i<n; i++)
    {
        for(int j = 0; j<n; j++)
            masC[i][j] = masA[i][j] + masB[i][j];
    }
}
void SumABParallelFor(double** masA, double** masB,double** masC, int n, int thread)
{
    omp_set_dynamic(0);      // Не менять число потоков во время исполнения
    omp_set_num_threads(thread);
    int i,j;
#pragma omp parallel for shared(masA,masB,masC) private(i,j)
    for(i = 0; i<n; i++)
    {
        for(j = 0; j<n; j++)
            masC[i][j] = masA[i][j] + masB[i][j];
    }
}
void SumABParallelSection(double** masA, double** masB,double** masC, int n, int thread)
{
    omp_set_dynamic(0);      // Не менять число потоков во время исполнения
    omp_set_num_threads(thread);
    int i,j;
#pragma omp parallel sections \
    shared(masA,masB,masC) private(i,j)
    {
#pragma omp section
        {
            for(i = 0; i<n/2; i++)
            {
                for(j = 0; j<n/2; j++)
                    masC[i][j] = masA[i][j] + masB[i][j];
            }
        }
#pragma omp section
        {
            for(i = n/2; i<n; i++)
            {
                for(j = n/2; j<n; j++)
                    masC[i][j] = masA[i][j] + masB[i][j];
            }
        }
    }
}

double SumC(double** masC, int n)
{
    double sum = 0;
    for(int i = 0; i<n; i++ )
    {
        for(int j = 0; j<n; j++)
        {
            sum += masC[i][j];
        }
    }
    return sum;
}
double SumCCritical(double** masC, int n, int thread)
{
    omp_set_dynamic(0);      // Не менять число потоков во время исполнения
    omp_set_num_threads(thread);
    double sum = 0;
    int i,j;
#pragma omp parallel for shared(masC) private(i,j)
    for(i = 0; i<n; i++ )
    {
        for(j = 0; j<n; j++)
        {
#pragma omp critical
            {
                sum += masC[i][j];
            }
        }
    }
    return sum;
}
double SumCReductor(double** masC, int n, int thread)
{
    omp_set_dynamic(0);      // Не менять число потоков во время исполнения
    omp_set_num_threads(thread);
    double sum = 0;
    int i,j;
#pragma omp parallel for  reduction(+:sum) \
    shared(masC) private(i,j)
    for(i = 0; i<n; i++ )
    {
        for(j = 0; j<n; j++)
        {
            sum += masC[i][j];
        }
    }
    return sum;
}
