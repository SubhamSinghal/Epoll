#include <inttypes.h>
#include <stdio.h>
#include <sys/timerfd.h>
#include <unistd.h>

int main()
{
        int fd;
        struct itimerspec timer = {
                .it_interval = {1, 0}, /* 1 second interval */
                .it_value = {1, 0}
        };
        uint64_t count;
        fd = timerfd_create(CLOCK_MONOTONIC, 0);
        if(fd < 0) {
                printf("error in creating timer");
        }
        timerfd_settime(fd, 0, &timer, NULL);

        for(;;){
                read(fd, &count, sizeof(count));
                printf("BEEP\n");
        }

        return 0;
}


