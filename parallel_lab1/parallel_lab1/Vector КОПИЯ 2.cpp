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

    int MasLen =1250000;
    double** masA = new double*[MasLen];
    double** masB = new double*[MasLen];
    double** masC = new double*[MasLen];

    for(int i = 0; i<MasLen;i++)
    {
        masA[i] = new double[1];
        masB[i] = new double[1];
        masC[i] = new double[1];
    }
    double timeStart, timeEnd;
    char* fname = "out.txt";
    ofstream SaveOut(fname,ios::out|ios::trunc);

    double FillingMasTime=0,SumABTime=0,SumCTime=0;

    double* FillingMasParallelTime=new double[3];
    double* SumABParallelForTime=new double[3];
    double* SumABParallelSectionTime=new double[3];
    double* SumCCriticalTime=new double[3];
    double* SumCReductorTime=new double[3];

    for(int k =0; k<20; k++)
    {
        cout<<k<<endl;
        timeStart = omp_get_wtime ();
        FillingMas(masA,MasLen);
        timeEnd = omp_get_wtime ();
        //cout<<"Время заполнения матрицы = "<<(timeEnd - timeStart)<<endl;
        //SaveOut<<(timeEnd - timeStart)<<endl;
        FillingMasTime += (timeEnd - timeStart);


        for (int i = 2; i<5; i++)
        {
            timeStart = omp_get_wtime ();
            FillingMasParallel(masB,MasLen,i);
            timeEnd = omp_get_wtime ();
            //cout<<"Количество потоков"<<i<<": ";
            //cout<<"Время паралельного заполнения матрицы = " \
            //   <<(timeEnd - timeStart)<<endl;
            //SaveOut<<(timeEnd - timeStart)<<endl;
            FillingMasParallelTime[i-2] += (timeEnd - timeStart);
        }

        timeStart = omp_get_wtime ();
        SumAB(masA, masB, masC,MasLen);
        timeEnd = omp_get_wtime ();
        /*cout<<"Время сложения двух матрицы = " \
           <<(timeEnd - timeStart)<<endl;
        SaveOut<<(timeEnd - timeStart)<<endl;*/
        SumABTime += (timeEnd - timeStart);

        for (int i = 2; i<5; i++)
        {
            timeStart = omp_get_wtime ();
            SumABParallelFor(masA, masB, masC,MasLen,i);
            timeEnd = omp_get_wtime ();
            /*cout<<"Количество потоков"<<i<<": ";
            cout<<"Время сложения двух матрицы  параллельно " \
                  "с использованием цикла FOR = " \
               <<(timeEnd - timeStart)<<endl;
            SaveOut<<(timeEnd - timeStart)<<endl;*/
            SumABParallelForTime[i-2] += (timeEnd - timeStart);
        }

        for (int i = 2; i<5; i++)
        {
            timeStart = omp_get_wtime ();
            SumABParallelSection(masA, masB, masC,MasLen, i);
            timeEnd = omp_get_wtime ();
            /*cout<<"Количество потоков"<<i<<": ";
            cout<<"Время сложения двух матрицы  параллельно " \
                  "с использованием секций = " \
               <<(timeEnd - timeStart)<<endl;
            SaveOut<<(timeEnd - timeStart)<<endl;*/
            SumABParallelSectionTime[i-2] += (timeEnd - timeStart);
        }

        timeStart = omp_get_wtime ();
        SumC(masC,MasLen);
        timeEnd = omp_get_wtime ();
        /*cout<<"Время поиска суммы элементов матрицы = " \
           <<(timeEnd - timeStart)<<endl;
        SaveOut<<(timeEnd - timeStart)<<endl;*/
        SumCTime += (timeEnd - timeStart);

        for (int i = 2; i<5; i++)
        {
            timeStart = omp_get_wtime ();
            SumCCritical(masC,MasLen,i);
            timeEnd = omp_get_wtime ();
            /*cout<<"Количество потоков"<<i<<": ";
            cout<<"Время поиска суммы элементов матрицы " \
                  "Параллельном с использованием критических секций = " \
               <<(timeEnd - timeStart)<<endl;
            SaveOut<<(timeEnd - timeStart)<<endl;*/
            SumCCriticalTime[i-2] += (timeEnd - timeStart);
        }

        for (int i = 2; i<5; i++)
        {
            timeStart = omp_get_wtime ();
            SumCReductor(masC,MasLen,i);
            timeEnd = omp_get_wtime ();
            /*cout<<"Количество потоков"<<i<<": ";
            cout<<"Время поиска суммы элементов матрицы " \
                  "Параллельном с использованием редукторов = " \
               <<(timeEnd - timeStart)<<endl;
            SaveOut<<(timeEnd - timeStart)<<endl;*/
            SumCReductorTime[i-2] += (timeEnd - timeStart);
        }
    }

    FillingMasTime/=20;
    for(int i=0;i<3;i++)
        FillingMasParallelTime[i]/=20;
    SumABTime/=20;
    for(int i=0;i<3;i++)
    {
        SumABParallelForTime[i]/=20;
        SumABParallelSectionTime[i]/=20;
    }
    SumCTime/=20;
    for(int i=0;i<3;i++)
    {
        SumCCriticalTime[i]/=20;
        SumCReductorTime[i]/=20;
    }

    SaveOut<<FillingMasTime/20<<endl;
    for(int i=0;i<3;i++)
        SaveOut<<FillingMasParallelTime[i]/20<<endl;
    SaveOut<<SumABTime/20<<endl;
    for(int i=0;i<3;i++)
    {
        SaveOut<<SumABParallelForTime[i]/20<<endl;
        SaveOut<<SumABParallelSectionTime[i]/20<<endl;
    }
    SaveOut<<SumCTime/20<<endl;
    for(int i=0;i<3;i++)
    {
        SaveOut<<SumCCriticalTime[i]/20<<endl;
        SaveOut<<SumCReductorTime[i]/20<<endl;
    }

    /*SaveOut<<FillingMasTime<<endl<<FillingMasParallelTime<<endl<< \
             SumABTime<<endl<<SumABParallelForTime<<endl<< \
             SumABParallelSectionTime<<endl<<SumCTime<<endl<< \
             SumCCriticalTime<<endl<<SumCReductorTime<<endl;*/

    SaveOut.close();

    for (int count = 0; count < MasLen; count++)
    {
        delete [] masA[count];
        delete [] masB[count];
        delete [] masC[count];
    }
    delete [] FillingMasParallelTime;
    delete [] SumABParallelForTime;
    delete [] SumABParallelSectionTime;
    delete [] SumCCriticalTime;
    delete [] SumCReductorTime;
}

void FillingMas(double** mas, int n)
{
    for(int i = 0; i<n; i++ )
    {
        //for(int j = 0; j<n; j++)
        //{
            double a = i+1;
            double b = 1/(i+1);
            mas[i][0] = pow(a,b) * cos(i+0);
        //}
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
            //for(j = 0; j<n; j++)
            //{
                double a = i+1;
                double b = 1/(j+1);
                mas[i][0] = pow(a,b) * cos(i+j);
            //}
        }
    }
}

void SumAB(double** masA, double** masB,double** masC, int n)
{
    for(int i = 0; i<n; i++)
    {
        //for(int j = 0; j<n; j++)
            masC[i][0] = masA[i][0] + masB[i][0];
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
        //for(j = 0; j<n; j++)
            masC[i][0] = masA[i][0] + masB[i][0];
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
                //for(j = 0; j<n/2; j++)
                    masC[i][0] = masA[i][0] + masB[i][0];
            }
        }
#pragma omp section
        {
            for(i = n/2; i<n; i++)
            {
                //for(j = n/2; j<n; j++)
                    masC[i][0] = masA[i][0] + masB[i][0];
            }
        }
    }
}

double SumC(double** masC, int n)
{
    double sum = 0;
    for(int i = 0; i<n; i++ )
    {
        //for(int j = 0; j<n; j++)
        //{
            sum += masC[i][0];
        //}
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
        //for(j = 0; j<n; j++)
        //{
#pragma omp critical
            {
                sum += masC[i][0];
            }
        //}
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
        //for(j = 0; j<n; j++)
        //{
            sum += masC[i][0];
        //}
    }
    return sum;
}
