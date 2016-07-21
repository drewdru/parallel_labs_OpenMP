#include <iostream>
#include <omp.h>

using namespace std;

int SecondFunc();
int main()
{
    omp_set_dynamic(0);// Не менять число потоков в параллельной часте программы
    for(int i =1; i<=8; i+=2)
    {
        if(i==3) ++i;
        omp_set_num_threads(i);
        SecondFunc();
    }
    return 0;
}

