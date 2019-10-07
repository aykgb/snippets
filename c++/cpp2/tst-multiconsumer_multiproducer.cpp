#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

#include <stdlib.h>

using namespace std;

static const int ITEM_REPOSITORY_SIZE = 10;
static const int ITEMS_TO_PRODUCE = 100;

struct ItemRepository {
    int item_buffer[ITEM_REPOSITORY_SIZE];
    size_t read_position;
    size_t write_position;
    size_t item_consumed_counter;
    size_t item_produced_counter;
    std::mutex mtx;
    std::mutex mtx_item_consumed_counter;
    std::mutex mtx_item_produced_counter;
    std::condition_variable repo_not_full;
    std::condition_variable repo_not_empty;
};

ItemRepository gItemRepository;

void InitItemReposity(ItemRepository *ir)
{
    ir->read_position = 0;
    ir->write_position = 0;
    ir->item_consumed_counter = 0;
    ir->item_produced_counter = 0;
}

void produce(ItemRepository *ir, int item)
{
    std::unique_lock<std::mutex> lck(ir->mtx);
    while(((ir->write_position + 1) % ITEM_REPOSITORY_SIZE) == ir->read_position) {
        std::cout << "Waiting an empty slot for producing item ...\n";
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
    std::unique_lock<std::mutex> lck(ir->mtx);
    while(ir->read_position == ir->write_position) {
        std::cout << "Waiting for an produced item ...\n";
        ir->repo_not_empty.wait(lck);
    }

    int item = ir->item_buffer[ir->read_position];
    ++ir->read_position;
    if(ir->read_position == ITEM_REPOSITORY_SIZE) {
        ir->read_position = 0;
    }

    ir->repo_not_full.notify_all();
    lck.unlock();

    return item;
}


void ProducerTask()
{
    bool ready_to_exit = false;
    while(true) {
        std::unique_lock<std::mutex> lck(gItemRepository.mtx_item_produced_counter);
        if(gItemRepository.item_produced_counter < ITEMS_TO_PRODUCE) {
            ++gItemRepository.item_produced_counter;
            produce(&gItemRepository, gItemRepository.item_produced_counter);
            std::cout << "ProducerTask Thread 0x" << hex << std::this_thread::get_id()
                      << dec << " is producing item #" << gItemRepository.item_produced_counter
                      << std::endl;
        } else {
            ready_to_exit = true;
        }

        lck.unlock();
        if(ready_to_exit) break;
    }
    std::cout << "ProducerTask thread 0x" << hex << std::this_thread::get_id()
              << "is exiting ..." << std::endl;
}

void ConsumerTask()
{
    bool ready_to_exit = false;
    while(true) {
        std::unique_lock<std::mutex> lck(gItemRepository.mtx_item_consumed_counter);
        if(gItemRepository.item_consumed_counter < ITEMS_TO_PRODUCE) {
            int item = consume(&gItemRepository);
            ++gItemRepository.item_consumed_counter;
            std::cout << "ConsumerTask thread 0x" << hex << std::this_thread::get_id()
                      << dec << " is consuming item #" << item << std::endl;
        } else {
            ready_to_exit = true;
        }

        lck.unlock();
        if(ready_to_exit) break;
    }

    std::cout << "ConsumerTask thread 0x" << hex << std::this_thread::get_id()
              << "is exiting ... " << std::endl;
}

int main()
{
    InitItemReposity(&gItemRepository);

    std::thread producer00(ProducerTask);
    std::thread producer01(ProducerTask);

    std::thread consumer00(ConsumerTask);
    std::thread consumer01(ConsumerTask);
    std::thread consumer02(ConsumerTask);
    std::thread consumer03(ConsumerTask);

    producer00.join();
    producer01.join();

    consumer00.join();
    consumer01.join();
    consumer02.join();
    consumer03.join();

    return 0;
}
