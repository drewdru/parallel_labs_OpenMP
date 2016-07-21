#include <iostream>
#include <omp.h>
#include <stdio.h>

using namespace std;

void HelloWorldParallel();//задание №1
void Vector();//задание №2
void Test();//задание №3
int main()
{
    setlocale(LC_ALL, "RUS");
    while (true)
    {
        int task;
        cout<<"Выберите задание (1-3): ";
        cin >> task;
        if(task==1) HelloWorldParallel();
        if(task==2) Vector();
        if(task==3) Test();
        char otv;
        cout<<"Продолжить? (y/n) : ";
        cin>>otv;
        if (otv == 'n') break;
    }


    return 0;
}

//считывает с клавиатуры количество создаваемых потоков
void HelloWorldParallel()
{
    int num_threads;
    cout << "Текущее число установленных потоков = "<<omp_get_num_threads()<<endl;
    cout<<"Сколько потоков создать? Ответ: "; cin>>num_threads;
    omp_set_num_threads(num_threads);
#pragma omp parallel
    {
        int i;
        i = omp_get_thread_num();
        printf("Привет от потока %d!\n",i);
    }
}

