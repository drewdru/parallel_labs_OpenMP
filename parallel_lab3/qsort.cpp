#include <iostream>
#include <omp.h>
using namespace std;
void quickSort(double *arr, int l, int r){
    int mid = (r + l) / 2;
    double pivot = arr[mid];
    int tl = l, tr = r;
    while(tl<=tr){
        while(arr[tl]<pivot) tl++;
        while(arr[tr]>pivot) tr--;
        if(tl<=tr){
            swap(arr[tl],arr[tr]);
            tr--;
            tl++;
        }
    }
    if(l<tr)quickSort(arr, l, tr);
    if(tl<r)quickSort(arr, tl, r);
}
double quickSort(double* arr, int size){
    int i;
    double *a = new double [size];
    for(i=0;i<size;i++) a[i] = arr[i];
    double start = omp_get_wtime();
    quickSort(a,0,size-1);
    delete []a;
    return omp_get_wtime()-start;
}
void quickParallelSort(double *arr, int l, int r){
    int mid = (r + l) / 2;
    double pivot = arr[mid];
    int tl = l, tr = r;
    while(tl<=tr){
        while(arr[tl]<pivot) tl++;
        while(arr[tr]>pivot) tr--;
        if(tl<=tr){
            swap(arr[tl],arr[tr]);
            tr--;
            tl++;
        }
    }
#pragma omp critical
    if(l<tr)quickParallelSort(arr, l, tr);
#pragma omp critical
    if(tl<r)quickParallelSort(arr, tl, r);
}
double quickParallelSort(double* arr, int size){
    int i;
    double *a = new double [size];
    for(i=0;i<size;i++) a[i] = arr[i];
    double start = omp_get_wtime();
#pragma omp parallel
#pragma omp single nowait
    quickParallelSort(a, 0, size-1);
    delete []a;
    return omp_get_wtime()-start;
}
