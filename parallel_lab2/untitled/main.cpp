#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iomanip>
using namespace std;

void ZadaniePi();
void MatrixOpt();
int main()
{
    setlocale(LC_ALL,"RUS");

    omp_set_num_threads(6);
    omp_set_dynamic(0);// Не менять число потоков во время исполнения

    char otv;
    cout<<"Выберите задание:\n";
    cout<<"1. Расчёт числа pi\n";
    cout<<"2. Работа с матрицами\n";
    cin>>otv;
    switch (otv)
    {
    case '1':
        ZadaniePi();
        break;
    case '2':
        MatrixOpt();
        break;
    default: break;
    }


    return 0;
}
