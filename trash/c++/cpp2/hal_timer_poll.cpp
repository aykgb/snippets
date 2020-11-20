
/*
 * File:   timer_poll.cpp
 * Author: Administrator
 */
#include <cstdlib>
#include <iostream>
#include "hal_timer_poll.h"

using namespace std;

timer::timer(const timer& ptimer)
{
    timer_internal = ptimer.timer_internal;
    cb = ptimer.cb;
    timer_id = ptimer.timer_id;
    repeat = ptimer.repeat;
    userdata = ptimer.userdata;
}

timer& timer::operator =(const timer& ptimer)
{
    if (this == &ptimer) {
        return *this;
    }
    timer_internal = ptimer.timer_internal;
    cb = ptimer.cb;
    timer_id = ptimer.timer_id;
    repeat = ptimer.repeat;
    userdata = ptimer.userdata;
    return *this;
}

int timer::timer_start()
{
    struct itimerspec ptime_internal = {{0, 0}, {0, 0}};
    ptime_internal.it_value.tv_sec = (int) timer_internal;
    ptime_internal.it_value.tv_nsec = (timer_internal - (int) timer_internal) * 1000000;
    if(repeat) {
        ptime_internal.it_interval.tv_sec = ptime_internal.it_value.tv_sec;
        ptime_internal.it_interval.tv_nsec = ptime_internal.it_value.tv_nsec;
    }

    timerfd_settime(timer_id, 0, &ptime_internal, NULL);
    return 0;
}

int timer::timer_stop()
{
    close(timer_id);
    return 0;
}

int timer::timer_modify_internal(double timer_internal)
{
    this->timer_internal = timer_internal;
    timer_start();
    return 0;
}

int timers_poll::timers_poll_add_timer(timer& ptimer)
{
    int timer_id = ptimer.timer_get_id();
    struct epoll_event ev;
    ev.data.fd = timer_id;
    ev.events = EPOLLIN | EPOLLET;
    timers_map[timer_id] = ptimer; //add or modify
    epoll_ctl (epfd, EPOLL_CTL_ADD, timer_id, &ev);
    ptimer.timer_start();

    return 0;
}

int timers_poll::timers_poll_del_timer(timer& ptimer)
{
    int timer_id = ptimer.timer_get_id();
    struct epoll_event ev;
    ev.data.fd = timer_id;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl (epfd, EPOLL_CTL_DEL, timer_id, &ev);
    timers_map.erase(timer_id);

    return 0;
}

void timers_poll::run()
{
    char buf[128] = {0};
    for (; active ; ) {
        struct epoll_event events[MAXFDS] = {{0}};
        int nfds = epoll_wait (epfd, events, MAXFDS, -1);
        for (int i = 0; i < nfds; ++i) {
            std::map<int, timer>::iterator itmp = timers_map.find(events[i].data.fd);
            if (itmp != timers_map.end()) {
                //timer ptimer = itmp->second;
                while (read(events[i].data.fd, buf, 128) > 0);
                itmp->second.get_user_callback()(itmp->second);
            }
        }
    }
}

int callback(timer& ptimer)
{
    //printf("timer id=%d:%s\n", ptimer.timer_get_id(), (char *) ptimer.timer_get_userdata());
    for(int i =0; i < 10000; i++);
    return 0;
}

int callback1(timer& ptimer)
{
    printf("timer id=%d:%s\n", ptimer.timer_get_id(), (char *) ptimer.timer_get_userdata());
    for(int i =0; i < 10000; i++);
    return 0;
    return 0;
}

void *thread_fun(void *data)
{
    timers_poll *my_timers = (timers_poll *)data;
    my_timers->run();
    return (void*)0;
}

/*
 *
 */
void timer_main(int argc, char **argv)
{
#if 0
    timers_poll my_timers(4);
    pthread_t thread_id = 0;
    pthread_create(&thread_id, NULL, thread_fun, &my_timers);

    timer timer1(0.5, callback, (void *) "hello 1", 1);
    timer timer2(0.5, callback, (void *) "hello 2", 1);
    timer timer3(0.5, callback, (void *) "hello 3", 1);
    timer timer4(0.5, callback, (void *) "hello 4", 1);
    // timer1.timer_start();
    // timer2.timer_start();
    my_timers.timers_poll_add_timer(timer1);
    my_timers.timers_poll_add_timer(timer2);
    my_timers.timers_poll_add_timer(timer3);
    my_timers.timers_poll_add_timer(timer4);
    // sleep(5);
    // my_timers.timers_poll_del_timer(timer2);
    // cout << "del complete" << endl;
    // timer1.timer_modify_internal(5.1);
    // timer2.timer_modify_internal(10.1);
    // cout << "modify complete" << endl;
    sleep(1);
    //my_timers.timers_poll_del_timer(timer2);

    //sleep(5);

    //my_timers.timers_poll_deactive();
    pthread_join(thread_id, NULL);
    return 0;
#endif
}


