#ifndef THREADPOOL_SRC_THREAD_H_
#define THREADPOOL_SRC_THREAD_H_

#include <thread>
#include <functional>
#include <memory>
#include <stdexcept>

class Thread {
public:
    Thread() { };
    virtual ~Thread() { };

};

class ThreadBase {
private:
    std::shared_ptr<std::thread> m_thread_ptr;

public:
    ThreadBase() :
        m_thread_ptr()
    {

    }

    virtual ~ThreadBase()
    {

    }

    void start()
    {
        if (m_thread_ptr == NULL) {
            m_thread_ptr.reset(new std::thread(std::bind(&run, this)));

        } else {
            throw std::runtime_error("multiple start");

        }

    }

    void join()
    {
        if (m_thread_ptr) {
            m_thread_ptr->join();

        }

    }

protected:
    virtual void run() = 0;

};

#endif /* THREADPOOL_SRC_THREAD_H_ */
