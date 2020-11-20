#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>

#include <unistd.h>
#include <stdlib.h>

using namespace std;

static const int ITEM_REPOSITORY_SIZE = 20; // Item buffer size.
static const int ITEMS_TO_PRODUCE = 1000;   // How many items we plan to produce.

struct ItemRepository {
    int item_buffer[ITEM_REPOSITORY_SIZE];    // Buffer of products, simulate a circle FIFO with the read_position/write_position using.
    size_t read_position;
    size_t write_position;
    size_t item_counter;       /* Used for multi-consumer. */
    std::mutex mtx;                     // Protect the item repository of product.
    std::mutex mtx_item_counter;        /* Sync multi-consumer */
    std::condition_variable repo_not_full;
    std::condition_variable repo_not_empty;
};

void InitItemRepository(ItemRepository *ir)
{
    ir->read_position = 0;
    ir->write_position = 0;
    ir->item_counter = 0;
}

ItemRepository gItemRepository;

void produce(ItemRepository *ir, int item)
{
    std::unique_lock<std::mutex> lck(ir->mtx);
    while(((ir->write_position + 1) % ITEM_REPOSITORY_SIZE ) == ir->read_position) {
        std::cout << "Producer is waiting for a empty slot ...\n";
        ir->repo_not_full.wait(lck);
    }

    ir->item_buffer[ir->write_position] = item;
    ++ir->write_position;

    if(ir->write_position == ITEM_REPOSITORY_SIZE) {
        ir->write_position = 0;
    }

    ir->repo_not_empty.notify_all();
    lck.unlock();
}

int consume(ItemRepository *ir)
{
    int data;
    std::unique_lock<std::mutex> lck(ir->mtx);
    while (ir->write_position == ir->read_position) {
        std::cout << "Consumer is waiting for items ...\n";
        ir->repo_not_empty.wait(lck);
    }

    data =  ir->item_buffer[ir->read_position];
    ++ir->read_position;

    if(ir->read_position == ITEM_REPOSITORY_SIZE) {
        ir->read_position = 0;
    }

    ir->repo_not_full.notify_all();
    lck.unlock();

    return data;
}

void ProducerTaskSingle()
{
    for(int i = 0; i < ITEMS_TO_PRODUCE; ++i) {
        std::cout << "Produce the item #" << i << " ..." << std::endl;
        produce(&gItemRepository, i);
    }
    std::cout << "Producer thread " << std::this_thread::get_id()
              << " is exiting..." << std::endl;
}

void ProducerTaskMultiple()
{
    bool ready_to_exit = false;
    while(true) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::unique_lock<std::mutex> lck(gItemRepository.mtx_item_counter);
        if(gItemRepository.item_counter < ITEMS_TO_PRODUCE) {
            ++gItemRepository.item_counter;
            produce(&gItemRepository, gItemRepository.item_counter);
            std::cout << "Producer thread " << std::this_thread::get_id()
                      << " is producing item #" << gItemRepository.item_counter
                      << " ..."<< std::endl;
        } else {
            ready_to_exit = true;
        }

        lck.unlock();
        if(ready_to_exit) break;
    }

    std::cout << "Producer thread " << std::this_thread::get_id()
              << "is exiting..." << std::endl;
}

void ConsumerTaskSingle()
{
    static int cnt = 0;
    while(true) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        int item = consume(&gItemRepository);
        std::cout << "Consume the item #" << item << " ..." << std::endl;
        if(++cnt == ITEMS_TO_PRODUCE) break;
    }
    std::cout << "Consumer thread " << std::this_thread::get_id()
              << " is exiting..." << std::endl;
}

void ConsumerTaskMultiple()
{
    bool ready_to_exit = false;
    while(true) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::unique_lock<std::mutex> lck(gItemRepository.mtx_item_counter);
        if(gItemRepository.item_counter < ITEMS_TO_PRODUCE) {
            int item = consume(&gItemRepository);
            ++gItemRepository.item_counter;
            std::cout << "Consumer thread " << std::this_thread::get_id()
                      << " is consuming the item #" << item << std::endl;
        } else {
            ready_to_exit = true;
        }
        lck.unlock();
        if(ready_to_exit) break;
    }

    std::cout << "Consumer thread " << std::this_thread::get_id()
              << " is exiting..." << std::endl;
}

int tst_000()
{
    InitItemRepository(&gItemRepository);
    std::thread producer(ProducerTaskSingle);
    std::thread consumer(ConsumerTaskSingle);

    producer.join();
    consumer.join();
    return 0;
}

int tst_001()
{
    InitItemRepository(&gItemRepository);
    std::thread producer(ProducerTaskSingle);
    std::thread consumer00(ConsumerTaskMultiple);
    std::thread consumer01(ConsumerTaskMultiple);
    std::thread consumer02(ConsumerTaskMultiple);
    std::thread consumer03(ConsumerTaskMultiple);

    producer.join();
    consumer00.join();
    consumer01.join();
    consumer02.join();
    consumer03.join();

    return 0;
}

int tst_002()
{
    InitItemRepository(&gItemRepository);
    std::thread producer00(ProducerTaskMultiple);
    std::thread producer01(ProducerTaskMultiple);
    std::thread producer02(ProducerTaskMultiple);
    std::thread producer03(ProducerTaskMultiple);
    std::thread consumer(ConsumerTaskSingle);

    producer00.join();
    producer01.join();
    producer02.join();
    producer03.join();
    consumer.join();
    return 0;
}

int main()
{
    // tst_000();
    // tst_001();
    tst_002();

    return 0;
}
