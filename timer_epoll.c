#include <inttypes.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <unistd.h>

int main(){
        int efd, tfd;
        struct itimerspec timer = {
                .it_interval = {1, 0}, /* 1 sec interval */
                .it_value = {1, 0}
        };

        uint64_t count;
        #define MAX_EVENTS 2
        struct epoll_event ev, events[MAX_EVENTS];

        int i, nr_events;

        /* no error checking */
        tfd = timerfd_create(CLOCK_MONOTONIC, 0);
        timerfd_settime(tfd, 0, &timer, NULL);


        /* create epoll */
        efd = epoll_create1(0);

        ev.events = EPOLLIN; /* set events we are interested in monitoring */
        ev.data.fd = tfd; /* set file descriptor which will be monitored by epoll */
        epoll_ctl(efd, EPOLL_CTL_ADD, tfd, &ev);

        while(1){
                nr_events = epoll_wait(efd, events, MAX_EVENTS, -1);
                for(i = 0; i < nr_events; i++) {
                        /* check for timer fd */
                        if(events[i].data.fd == tfd) {
                                read(tfd, &count, sizeof(count));
                                printf("BEEP\n");
                        }
                }
        }

        return 0;
}

