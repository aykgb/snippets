#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

class ThreadPool;

class Worker {
public:
    Worker(ThreadPool &s) : pool(s) { }
    void operator() ();
private:
    ThreadPool &pool;
};

class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F>
    void enquene(F f);
    ~ThreadPool();
private:
    friend class Worker;

    // Need to keep track of threads so we can join them
    std::vector<std::thread > workers;

    // the task queue
    std::deque<std::function<void()> > tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

void Worker::operator()()
{
    std::function<void()> task;
    while(true) {
        {
            //acquire lock
            std::unique_lock<std::mutex> lock(pool.queue_mutex);

            // look for a work item
            while(!pool.stop && pool.tasks.empty()) {
                pool.condition.wait(lock);
            }
            if(pool.stop) return; // exit if the pool is stopped.

            // get the task from the queue
            task = pool.tasks.front();
            pool.tasks.pop_front();

        } // release lock

        // execute the tasks
        task();
    }
}

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
    : stop(false)
{
    for(size_t i = 0; i < threads; ++i)
        workers.push_back(std::thread(Worker(*this)));
}

// the destrutor joins all threads
ThreadPool::~ThreadPool()
{
    stop = true;
    condition.notify_all();

    // join them
    for(size_t i = 0; i < workers.size(); ++i)
        workers[i].join();
}

// add new work item to the pool
template<class F>
void ThreadPool::enquene(F f)
{
    {
        //acquire lock
        std::unique_lock<std::mutex> lock(queue_mutex);

        // add the tasks
        tasks.push_back(std::function<void()>(f));
    }// release lock

    // wake up one thread
    condition.notify_one();
}

int main()
{
    return 0;
}
