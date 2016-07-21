#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iomanip>
using namespace std;

void Zap(double** Mas, int P, int L);
void ZapParallelFor(double** Mas, int P, int L);

void AxB(double** Mas1, int m, int n, double** Mas2, int nn, int k, double** Mas3);

void AxBParallelForReductor(double** Mas1, int m, int n, double** Mas2, \
                            int nn, int k, double** Mas3);
void AxBParallelForCritical(double** Mas1, int m, int n, double** Mas2, \
                            int nn, int k, double** Mas3);
void AxBPiParallelForOrdered(double** Mas1, int m, int n, double** Mas2, \
                             int nn, int k, double** Mas3);

static long M = 150 ;
static long N = 150 ;
static long K = 150 ;

void MatrixOpt()
{
    double timeStart, timeEnd,time;
    const char* fname = "out.txt";
    ofstream SaveOut(fname,ios::out|ios::trunc);

    double** masA = new double*[M];
    for(int i =0; i<M; i++)
    {
        masA[i] = new double[N];
    }

    double** masB = new double*[N];
    for(int i =0; i<N; i++)
    {
        masB[i] = new double[K];
    }

    double** masC = new double*[M];
    for(int i =0; i<M; i++)
    {
        masC[i] = new double[K];
    }


    time = 0;
    for(int i = 0; i<10; i++)
    {
        timeStart = omp_get_wtime ();
        Zap(masA,M,N);
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=10;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<10; i++)
    {
        timeStart = omp_get_wtime ();
        ZapParallelFor(masB,N,K);
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=10;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<10; i++)
    {
        timeStart = omp_get_wtime ();
        AxB(masA,M,N, masB,N,K, masC);
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=10;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<10; i++)
    {
        timeStart = omp_get_wtime ();
        AxBParallelForReductor(masA,M,N, masB,N,K,masC);
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=10;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;
cout<<"!!!!\n";
    time = 0;
    for(int i = 0; i<10; i++)
    {
        timeStart = omp_get_wtime ();
        AxBParallelForCritical(masA,M,N, masB,N,K,masC);
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=10;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<10; i++)
    {
        timeStart = omp_get_wtime ();
        AxBPiParallelForOrdered(masA,M,N, masB,N,K,masC);
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=10;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    SaveOut.close();
}

void Zap(double** Mas, int P, int L)
{
    for(int i = 0; i<P; i++ )
    {
        for(int j = 0; j<L; j++)
        {
            double a = i+1;
            double b = 1/(j+1);
            Mas[i][j] = pow(a,b) * cos(i+j);
        }
    }
}

void ZapParallelFor(double** Mas, int P, int L)
{
    int i,j;
#pragma omp parallel for shared(Mas) private(i)
    for (i=0; i< P; i++)
    {
#pragma omp parallel for shared(Mas) firstprivate(i) private(j)
        for(j = 0; j<L; j++)
        {
            double a = i+1;
            double b = 1/(j+1);
            Mas[i][j] = pow(a,b) * cos(i+j);
        }
    }
}

// ///////////////////////////////////////////////////////////////////////////////////
void AxB(double** Mas1, int m, int n, double** Mas2, int nn, int k, double** Mas3)
{
    double sum = 0.0;

    if(n==nn)
    {
        for (int row = 0; row < m; row++)
        {
            for (int col = 0; col < n; col++)
            {
                for (int inner = 0; inner < k; inner++)
                {
                    sum += Mas1[row][inner] * Mas2[inner][col];
                }
                Mas3[row][col] = sum;
                sum = 0;
            }
        }
    }
    else cout<<"перемножение не возможно, количество";
}

void AxBParallelForReductor(double** Mas1, int m, int n, double** Mas2,\
                            int nn, int k, double** Mas3)
{
    double sum = 0.0;
    int row,col,inner;
    if(n==nn)
    {
#pragma omp parallel for shared (Mas3) private(row, col,inner)\
    schedule(static, m / omp_get_num_threads())
        for (row = 0; row < m; row++)
        {
#pragma omp parallel for shared (Mas3) private(col,inner)\
        firstprivate(row) schedule(static, m / omp_get_num_threads())
            for (col = 0; col < n; col++)
            {
#pragma omp parallel for private(inner) \
    firstprivate(row,col) reduction(+:sum)
                for (inner = 0; inner < k; inner++)
                    sum = Mas1[row][inner] * Mas2[inner][col];

                Mas3[row][col] = sum;
                sum =0;
            }
        }
    }
    else cout<<"перемножение не возможно, количество";

}

void AxBParallelForCritical(double** Mas1, int m, int n, double** Mas2,\
                            int nn, int k, double** Mas3)
{
    double sum = 0.0;
    int row,col,inner;
    if(n==nn)
    {
        for (row = 0; row < m; row++)
        {
            for (col = 0; col < n; col++)
            {
#pragma omp parallel for private(inner) \
    firstprivate(row,col)
                for (inner = 0; inner < k; inner++)
                {
#pragma omp critical
                    {
                        sum = sum + Mas1[row][inner] * Mas2[inner][col];
                    }
                }

                Mas3[row][col] = sum;
                sum =0;
            }
        }
    }
    else cout<<"перемножение не возможно, количество";
}

void AxBPiParallelForOrdered(double** Mas1, int m, int n, double** Mas2,\
                             int nn, int k, double** Mas3)
{
    double sum = 0.0;
    int row,col,inner;
    if(n==nn)
    {
        for (row = 0; row < m; row++)
        {
            for (col = 0; col < n; col++)
            {
#pragma omp parallel for ordered private(inner) \
    firstprivate(row,col)
                for (inner = 0; inner < k; inner++)
                {
#pragma omp	ordered
                    {
                        sum = sum + Mas1[row][inner] * Mas2[inner][col];
                    }
                }

                Mas3[row][col] = sum;
                sum =0;
            }
        }
    }
    else cout<<"перемножение не возможно, количество";
}
