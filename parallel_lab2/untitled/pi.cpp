#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iomanip>
using namespace std;

static long num_steps=20000000;
double step, pi;
void PiPosl()
{
    int i;
    double x, sum = 0.0;
    step = 1.0/(double) num_steps;
    for (i=0; i< num_steps; i++)
    {
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0 + x*x);
    }
    pi = step * sum;
    printf("Pi = %f\n",pi);
}

void PiParallelSection()
{
    int i;
    double x, sum = 0.0;
    step = 1.0/(double) num_steps;
#pragma omp parallel sections firstprivate(step)\
                    private(i,x)\
                    reduction(+:sum)
    {
#pragma omp section
        for (i=0; i< num_steps/4; i++)
        {
            x = (i+0.5)*step;
            sum = sum + 4.0/(1.0 + x*x);
        }
#pragma omp section
        for (i=num_steps/4; i< (num_steps/4)*2; i++)
        {
            x = (i+0.5)*step;
            sum = sum + 4.0/(1.0 + x*x);
        }
#pragma omp section
        for (i=(num_steps/4)*2; i< (num_steps/4)*3; i++)
        {
            x = (i+0.5)*step;
            sum = sum + 4.0/(1.0 + x*x);
        }
#pragma omp section
        for (i=(num_steps/4)*3; i< num_steps; i++)
        {
            x = (i+0.5)*step;
            sum = sum + 4.0/(1.0 + x*x);
        }
    }
    pi = step * sum;
    printf("Pi = %f\n",pi);
}

void PiParallelForReductor()
{
    int i;
    double x, sum = 0.0;
    step = 1.0/(double) num_steps;
#pragma omp parallel for firstprivate(step)\
                    private(i,x)\
                    reduction(+:sum)
    for (i=0; i< num_steps; i++)
    {
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0 + x*x);
    }
    pi = step * sum;
    printf("Pi = %f\n",pi);
}

void PiParallelForCritical()
{
    int i;
    double x, sum = 0.0;
    step = 1.0/(double) num_steps;
#pragma omp parallel for firstprivate(step)\
                    private(i,x)
    for (i=0; i< num_steps; i++)
    {
        x = (i+0.5)*step;

#pragma omp critical
        {
            sum = sum + 4.0/(1.0 + x*x);
        }
    }
    pi = step * sum;
    printf("Pi = %f\n",pi);
}

void PiParallelSingle()
{
    int i;
    double x, sum = 0.0;
    step = 1.0/(double) num_steps;
#pragma omp parallel firstprivate(step)\
                    private(i,x)
    for (i=0; i< num_steps; i++)
    {
        x = (i+0.5)*step;
#pragma omp	single
        {
            sum = sum + 4.0/(1.0 + x*x);
        }
    }
    pi = step * sum;
    printf("Pi = %f\n",pi);
}

void PiParallelForOrdered()
{
    int i;
    double x, sum = 0.0;
    step = 1.0/(double) num_steps;
#pragma omp parallel for  ordered \
            firstprivate(step)\
            private(i,x)
    for (i=0; i< num_steps; i++)
    {
        x = (i+0.5)*step;
#pragma omp	ordered
        {
            sum = sum + 4.0/(1.0 + x*x);
        }
    }
    pi = step * sum;
    printf("Pi = %f\n",pi);
}

void ZadaniePi()
{
    double timeStart, timeEnd,time;
    const char* fname = "out.txt";
    ofstream SaveOut(fname,ios::out|ios::trunc);

    time = 0;
    for(int i = 0; i<20; i++)
    {
        timeStart = omp_get_wtime ();
        PiPosl();
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=20;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<20; i++)
    {
        timeStart = omp_get_wtime ();
        PiParallelSection();
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=20;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<20; i++)
    {
        timeStart = omp_get_wtime ();
        PiParallelForReductor();
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=20;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<20; i++)
    {
        timeStart = omp_get_wtime ();
        PiParallelForCritical();
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=20;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<20; i++)
    {
        timeStart = omp_get_wtime ();
        PiParallelSingle();
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=20;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    time = 0;
    for(int i = 0; i<20; i++)
    {
        timeStart = omp_get_wtime ();
        PiParallelForOrdered();
        timeEnd = omp_get_wtime ();
        time+= (timeEnd - timeStart);
    }
    time/=20;
    SaveOut<<time<<endl;
    cout<<"Time = "<<time<<endl;

    SaveOut.close();
}
