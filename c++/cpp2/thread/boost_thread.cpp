#include <boost/thread.hpp>
#include <cstdio>

boost::mutex mutex;

void threadA()
{
    for (int i = 10; i > 0; ) {
        boost::mutex::scoped_lock lock(mutex);
        {
            fprintf(stderr, "Locked by A\n");
            usleep(100000);
            fprintf(stderr, "Unlocked by A\n");
            i--;
        }
    }
}

void threadB()
{
    for (int i = 10; i > 0; ) {
        boost::mutex::scoped_lock lock(mutex, boost::try_to_lock);
        if (lock) {
            fprintf(stderr, "Locked by B\n");
            usleep(100000);
            fprintf(stderr, "Unlocked by B\n");
            i--;
        } else {
            fprintf(stderr, "Lock failed in B\n");
            usleep(100000);
        }
    }
}

int main(int argc, char **argv)
{
    boost::thread a(threadA);
    boost::thread b(threadB);
    a.join();
    b.join();
}
