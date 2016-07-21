#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXSTACK 2048

using namespace std;

void BubbleSort (double* pData, int Size);

void OddEvenSort( double*pData, int Size);
void ParallelOddEvenSort( double*pData, int Size);
void ParallelOddEvenSortScheduleStatic( double*pData, int Size);

void ShellSort(double*pData, int size);
void ShellSortParallel(double*pData, int n);
void ShellSortParallelScheduleStatic( double*pData, int n);

/*void q_sort(double*arr, int size);
double quickSort(double* arr, int size);
double quickParallelSort(double* arr, int size);
void q_SortParral(double *masA, int n);*/
void q_sort(double* masA, const int n);
void  q_SortParral(double* masA, const int n/*, int theard*/);
int Sort(double *&mas, double *&temp, int n)
{
    const char* fname = "out.txt";
    ofstream SaveOut(fname,ios::out|ios::app);

    /*srand(time(NULL));
    int n =10000;
    double *mas = new double[n];
    double *temp = new double[n];
    int i;
#pragma omp parallel for shared(mas)
    for(i=0; i<n;i++)
        mas[i] = (double) (rand() % ( 10000 - \
                (-10000) + 1)+(-10000))/100;
    memcpy( temp, mas, n*sizeof(double) );*/

    /*double time = omp_get_wtime();
    for (int i=0;i<10;i++)
        BubbleSort (mas, n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;

    memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    for (int i=0;i<10;i++)
        OddEvenSort(mas, n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;

    memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    for (int i=0;i<10;i++)
        ParallelOddEvenSort(mas, n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;

    memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    for (int i=0;i<10;i++)
        ParallelOddEvenSortScheduleStatic(mas, n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;

    memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    for (int i=0;i<10;i++)
        ShellSort(mas, n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;

    memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    for (int i=0;i<10;i++)
        ShellSortParallel(mas, n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;

    memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    for (int i=0;i<10;i++)
        ShellSortParallelScheduleStatic(mas, n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;*/

    /*memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    for (int i=0;i<10;i++)
        q_sort(mas,n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;
    SaveOut<<endl;*/
    omp_set_num_threads(1);
    memcpy( mas, temp, n*sizeof(double) );
    double time = omp_get_wtime();
    for (int i=0;i<10;i++)
        q_sort(mas,n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;
    SaveOut<<endl;
    return -1;

    /*memcpy( mas, temp, n*sizeof(double) );
    time = 0;
    //for (int i=0;i<10;i++)
        time+=quickParallelSort(mas, n);
    //time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;
    SaveOut<<endl;*/

    memcpy( mas, temp, n*sizeof(double) );
    time = omp_get_wtime();
    for (int i=0;i<10;i++)
        q_SortParral(mas, n);
    time = omp_get_wtime() - time;
    cout<<"time = "<<time/10<<"; ";
    SaveOut<<time/10<<endl;
    cout<<endl;
    SaveOut<<endl;
    memcpy( mas, temp, n*sizeof(double) );
    SaveOut.close();

    return 0;
}

void q_sort(double* masA, const int n)
{
    int i = 0, j = n;
    double pivot = masA[n / 2]; // âûáîð îïîðíîãî ýëåìåíòà

    do {
        while (masA[i] < pivot) i++;
        while (masA[j] > pivot) j--;

        if (i <= j) {
            swap(masA[i], masA[j]);
            i++; j--;
        }
    } while (i <= j);

    if (j > 0) q_sort(masA, j);
    if (n > i) q_sort(masA + i, n - i);
}

void  q_SortParral(double* masA, const int n/*, int theard*/)
{
    int i = 0, j = n;
    double pivot = masA[n / 2]; // âûáîð îïîðíîãî ýëåìåíòà
#pragma omp parallel shared(masA) num_threads(omp_get_num_threads())
    {
        do {
            while (masA[i] < pivot) i++;
            while (masA[j] > pivot) j--;

            if (i <= j) {
                swap(masA[i], masA[j]);
                i++; j--;
            }
        } while (i <= j);

#pragma omp barrier  //parallel num_threads(theard)
        {
            if (j > 0) q_sort(masA, j); }
#pragma omp barrier //parallel num_threads(theard)
        {
        if (n > i) q_sort(masA + i, n - i); }
    }
}


void swap(double& a,double& b)
{
    double temp;
    temp = b;
    b = a;
    a = temp;
}

//Функция для алгоритма пузырьковой сортировки
void BubbleSort (double* pData, int Size)
{
    for (int i=0; i<Size-1; i++)
        for (int j=1; j<Size-i; j++)
            if (pData[j-1]>pData[j])
                swap(pData[j-1],pData[j]);
}

//Функция для алгоритма чет-нечетной перестановки
void OddEvenSort( double*pData, int Size)
{
    int upper_bound;
    if (Size%2==0)
        upper_bound = Size/2-1;
    else
        upper_bound = Size/2;

    for (int i=0; i<Size; i++)
    {
        if (i%2 == 0)
        {
            //четная итерация
            for (int j=0; j<Size/2; j++)
                if(pData[2*j]>pData[2*j+1])
                    swap(pData[2*j], pData[2*j+1]);
        }
        else
        {
                    //нечетная итерация
                    for (int j=0; j<upper_bound; j++)
                        if(pData[2*j+1]>pData[2*j+2])
                            swap(pData[2*j+1],pData[2*j+2]);
        }
    }
}
//Функция для алгоритма чет-нечетной перестановки
void ParallelOddEvenSort( double*pData, int Size)
{
    int upper_bound;
    if (Size%2==0)
        upper_bound = Size/2-1;
    else
        upper_bound = Size/2;
    for (int i=0; i<Size; i++)
    {
        int j=0;
        if (i%2 == 0) // четная итерация
        {
#pragma omp parallel for shared(pData)
            for (j=0; j<Size/2; j++)
                if(pData[2*j]>pData[2*j+1])
                    swap(pData[2*j], pData[2*j+1]);
        }
        else // нечетная итерация
        {
#pragma omp parallel for shared(pData)
            for (j=0; j<upper_bound; j++)
                if(pData[2*j+1]>pData[2*j+2])
                    swap(pData[2*j+1],pData[2*j+2]);
        }
    }
}
void ParallelOddEvenSortScheduleStatic( double*pData, int Size)
{
    int upper_bound;
    if (Size%2==0)
        upper_bound = Size/2-1;
    else
        upper_bound = Size/2;
    for (int i=0; i<Size; i++)
    {
        int j=0;
        if (i%2 == 0) // четная итерация
        {
#pragma omp parallel for shared(pData)\
    schedule(static, 6)
            for (j=0; j<Size/2; j++)
                if(pData[2*j]>pData[2*j+1])
                    swap(pData[2*j], pData[2*j+1]);
        }
        else // нечетная итерация
        {
#pragma omp parallel for shared(pData)\
    schedule(static, 6)
            for (j=0; j<upper_bound; j++)
                if(pData[2*j+1]>pData[2*j+2])
                    swap(pData[2*j+1],pData[2*j+2]);
        }
    }
}

//Последовательный алгоритм сортировки Шелла
void ShellSort(double*pData, int size)
{
    int step = size / 2;//инициализируем шаг.
    while (step > 0)//пока шаг не 0
    {
        for (int i = 0; i < (size - step); i++)
        {
            int j = i;
            //будем идти начиная с i-го элемента
            while (j >= 0 && pData[j] > pData[j + step])
                //пока не пришли к началу массива
                //и пока рассматриваемый элемент больше
                //чем элемент находящийся на расстоянии шага
            {
                //меняем их местами
                double temp = pData[j];
                pData[j] = pData[j + step];
                pData[j + step] = temp;
                j--;
            }
        }
        step = step / 2;//уменьшаем шаг
    }
}
void insertionsort(double*pData, int n, int stride) {
    for (int j=stride; j<n; j+=stride) {
        double key = pData[j];
        int i = j - stride;
        while (i >= 0 && pData[i] > key) {
            pData[i+stride] = pData[i];
            i-=stride;
        }
        pData[i+stride] = key;
    }
}
void ShellSortParallel(double*pData, int n)
{
    int i, m;

    for(m = n/2; m > 0; m /= 2)
    {
            #pragma omp parallel for shared(pData,m,n) private (i) default(none)
            for(i = 0; i < m; i++)
                insertionsort(&(pData[i]), n-i, m);
    }
}
void ShellSortParallelScheduleStatic( double*pData, int n)
{
    int i, m;

    for(m = n/2; m > 0; m /= 2)
    {
#pragma omp parallel for shared(pData,m,n) private (i) default(none)\
    schedule(static, 6)
            for(i = 0; i < m; i++)
                insertionsort(&(pData[i]), n-i, m);
    }
}

/*void q_sort(double*arr, int size)
{
    int i, j;   			// указатели, участвующие в разделении
    int lb, ub;  		// границы сортируемого в цикле фрагмента
    int lbstack[MAXSTACK], ubstack[MAXSTACK]; // стек запросов
                        // каждый запрос задается парой значений,
                        // а именно: левой(lbstack) и правой(ubstack)
                        // границами промежутка
    int stackpos = 1;   	// текущая позиция стека
    int ppos;            // середина массива
    double pivot;              // опорный элемент
    int temp;

    lbstack[1] = 0;
    ubstack[1] = size-1;

    do {
        // Взять границы lb и ub текущего массива из стека.
        lb = lbstack[ stackpos ];
        ub = ubstack[ stackpos ];
        stackpos--;
        do {
            // Шаг 1. Разделение по элементу pivot
             ppos = ( lb + ub ) >> 1;
            i = lb;
            j = ub;
            pivot = arr[ppos];

            do {
                while ( arr[i] < pivot ) i++;
                while ( pivot < arr[j] ) j--;

                if ( i <= j ) {
                    temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                    i++;
                    j--;
                }
            } while ( i <= j );

            // Сейчас указатель i указывает на начало правого подмассива,
            // j - на конец левого (см. иллюстрацию выше), lb ? j ? i ? ub.
            // Возможен случай, когда указатель i или j выходит за границу массива

            // Шаги 2, 3. Отправляем большую часть в стек  и двигаем lb,ub
            if ( i < ppos )
            {     // правая часть больше
                if ( i < ub )
                {     //  если в ней больше 1 элемента - нужно
                    stackpos++;       //  сортировать, запрос в стек
                    lbstack[ stackpos ] = i;
                    ubstack[ stackpos ] = ub;
                }
                ub = j;     //  следующая итерация разделения
                            //  будет работать с левой частью
            }
            else
            {       	    // левая часть больше
                if ( j > lb )
                {
                    stackpos++;
                    lbstack[ stackpos ] = lb;
                    ubstack[ stackpos ] = j;
                }
                lb = i;
            }
        } while ( lb < ub );        // пока в меньшей части более 1 элемента
    } while ( stackpos != 0 );    // пока есть запросы в стеке
}
*/
