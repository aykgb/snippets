import time

from multiprocessing.managers import BaseManager


class QueueManger(BaseManager):
    pass

# Only name is provided when registering, since the QueueManger is getting
# from internet.
QueueManger.register('get_task_queue')
QueueManger.register('get_result_queue')

# Link to the server, which is the machine that runs the task_master.py
server_addr = '127.0.0.1'
print('Connect to server %s...' % server_addr)

# Notice that keep the port and codes be totally same with the setting of
# task_master.py


def client_main():
    m = QueueManger(address=(server_addr, 5000), authkey=b'abc')

    # Link it from internet
    m.connect()

    # Get the object of Queue
    task = m.get_task_queue()
    result = m.get_result_queue()

    # Get the task from the task_queue, and writes the result to result_queue.
    for i in range(10):
        try:
            n = task.get(timeout=1)
            print('run task %d * %d...' % (n, n))
            r = '%d * %d = %d' % (n, n, n*n)
            time.sleep(1)
            result.put(r)

        except task.Empty:
            print('task queue is empty.')

    # ends the process.
    print('worker exit.')


if __name__ == '__main__':
    client_main()
