#include "thread_pool.h"
#include <fstream>

int main()
{
    typedef int T;

    CULL size = 100000000;
    T *arr;
    arr = new T[size];

    for(unsigned i = 0; i < size; ++i)
        arr[i] = size - i;

    auto begin = std::chrono::high_resolution_clock::now();

    thread_pool<T> sort(arr, size);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = end - begin;
    cout << "Duration: " << dur.count() << "\n";

    std::ofstream out("shell.txt", std::ios::app);
    out << "Size: " << size << "\nDuration: " << dur.count() << "\n\n";
    out.close();
    delete[] arr;

    return 0;
}

