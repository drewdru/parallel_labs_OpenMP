#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iomanip>
/*#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>

using namespace std;
void q_sort(double*arr, long size);
void quickSort(double *vData, int posStart, int posEnd);

void quicksort(double *a, int p, int r);
void SORT()
{
    srand(time(NULL));
    int n =10000;
    double *mas = new double[n];
    double *temp = new double[n];
    int i;
#pragma omp parallel for shared(mas)
    for(i=0; i<n;i++)
        mas[i] = (double) (rand() % ( 10000 - \
                                      (-10000) + 1)+(-10000))/100;
    memcpy( temp, mas, n*sizeof(double) );

    omp_set_num_threads(4);
    //omp_set_dynamic(0);// Не менять число потоков в параллельной часте программы
    //    for(int i =2; i<3; i+=3)
    //    {
    //        omp_set_num_threads(i);
    //        Sort(mas,temp,n);
    //    }
    double time;
    time = omp_get_wtime();
    q_sort(mas,n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time<<endl;

    memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    quicksort(mas, 0, n-1);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time<<endl;

    delete[] temp;
    delete[] mas;
}

int partition(double * a, int p, int r)
{
    int lt[r-p];
    int gt[r-p];
    int i;
    int j;
    int key = a[r];
    int lt_n = 0;
    int gt_n = 0;

#pragma omp parallel for
    for(i = p; i < r; i++){
        if(a[i] < a[r]){
            lt[lt_n++] = a[i];
        }else{
            gt[gt_n++] = a[i];
        }
    }

    for(i = 0; i < lt_n; i++){
        a[p + i] = lt[i];
    }

    a[p + lt_n] = key;

    for(j = 0; j < gt_n; j++){
        a[p + lt_n + j + 1] = gt[j];
    }

    return p + lt_n;
}

void quicksort(double * a, int p, int r)
{
    int div;

    if(p < r){
        div = partition(a, p, r);
#pragma omp parallel sections
        {
#pragma omp section
            quicksort(a, p, div - 1);
#pragma omp section
            quicksort(a, div + 1, r);

        }
    }
}*/
