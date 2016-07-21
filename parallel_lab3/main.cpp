#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int Sort(double *&mas,double *&temp, int n);
int SecondFunc();
void SORT();
int analys();
int main()
{
    /*srand(time(NULL));
    int n =10000000;
    double *mas = new double[n];
    double *temp = new double[n];
    int i;
#pragma omp parallel for shared(mas)
    for(i=0; i<n;i++)
        mas[i] = (double) (rand() % ( 10000 - \
                (-10000) + 1)+(-10000))/100;
    memcpy( temp, mas, n*sizeof(double) );

    omp_set_dynamic(0);// Не менять число потоков в параллельной часте программы
    for(int i =1; i<8; i+=3)
    {
        omp_set_num_threads(i);
        Sort(mas,temp,n);
    }
    delete[] temp;
    delete[] mas;

    /*omp_set_dynamic(0);// Не менять число потоков в параллельной часте программы
    for(int i =1; i<8; i+=3)
    {
        omp_set_num_threads(i);
        SecondFunc();
    }*/
    //SORT();
    analys();
    return 0;
}

