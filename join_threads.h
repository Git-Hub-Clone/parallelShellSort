#ifndef JOIN_THREADS
#define JOIN_THREADS

#include <vector>
#include <thread>

class JoinThreads
{
public:
    explicit JoinThreads(std::vector<std::thread>& threads_) :
        threads(threads_)
    {}

    ~JoinThreads()
    {
        for(unsigned i = 0; i < threads.size(); ++i)
        {
            if (threads[i].joinable())
                threads[i].join();
        }
    }

private:
    std::vector<std::thread>& threads;
};

#endif // JOIN_THREADS

