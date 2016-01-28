#ifndef THREAD_POOL
#define THREAD_POOL

#include <iostream>
#include <atomic>
#include "queue.h"
#include "join_threads.h"
#include "steps_by_sejvik.h"

using std::cout;
typedef const unsigned long long CULL;

template <class T_ARR>
class thread_pool
{
    std::atomic_bool        done;
    std::atomic_bool        can_do;
    std::atomic_int         thread_c;
    std::atomic_int         wait_thread_c;

    struct work_block
    {
        int step;
        int begin_point;
    };

    Queue<work_block>       work_queue;
    std::vector<std::thread> threads;
    JoinThreads joiner;

    T_ARR * const array;
    CULL size;

    void do_sort()
    {
        ++thread_c;

        while(!(done.load() && work_queue.is_empty()))
        {
            std::shared_ptr<work_block> block = work_queue.pop();

            if (!block)
            {
                std::this_thread::yield();
            }

            else
            {
                int step = block.get()->step;

                if (step == -1)
                {
                    can_do.store(false);
                    continue;
                }

                if (!can_do.load())
                {
                    ++wait_thread_c;

                    while(wait_thread_c.load() != thread_c.load())
                    {
                        std::this_thread::sleep_for(std::chrono::microseconds(100));
                    }

                    if (!can_do.load())
                    {
                        can_do.store(true);
                    }
                    --wait_thread_c;
                }

                int j;

                for(int i = step + block.get()->begin_point; i < size; i += step)
                {
                    T_ARR temp = array[i];

                    for(j = i - step; j >= 0 && array[j] > temp; j -= step)
                    {
                        array[j + step] = array[j];
                    }
                    array[j + step] = temp;
                }
            }
        }
        --thread_c;
    }

    void create_work()
    {
        int ind = 0;
        while(getSejvikStep(++ind) * 3 < size) ;

        for(int index = --ind; index >= 0; --index)
        {
            int step = getSejvikStep(index);

            for(int for_step = 0; for_step < step; ++for_step)
            {
                work_block block = {
                    step,
                    for_step
                };
                work_queue.push(block);
            }

            if (index)
            {
                work_block block = {
                    -1, -1
                };
                work_queue.push(block);
            }
        }
        done.store(true);
        do_sort();
    }

public:
    thread_pool() = delete;
    thread_pool(const thread_pool &) = delete;
    thread_pool& operator= (const thread_pool &) = delete;

    thread_pool(T_ARR *& __array, CULL __size) :
        done(false),
        can_do(true),
        thread_c(0),
        wait_thread_c(0),
        joiner(threads),
        array(__array),
        size(__size)
    {
        int thread_count = std::thread::hardware_concurrency() - 1;

        if (thread_count < 1)
            thread_count = 1;

        bool well = true;

        try
        {
            for(unsigned i = 0; i < thread_count; ++i)
                threads.push_back(std::thread(&thread_pool::do_sort, this));
        }

        catch(...)
        {
            well = false;
            cout << "****** oops ******\nAll or some threads don't created\nand I can't to sort, sorry. =(\n";
        }

        if (well)
        {
            create_work();
        }
        joiner.~JoinThreads();
    }
};

#endif // THREAD_POOL




































