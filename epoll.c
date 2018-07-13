/*
 *epoll.c
 */
/********************************头文件****************************************/
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

/********************************宏定义****************************************/
#define DATA_MAX_LEN        500
#define EPOLL_MAX_EVENTS    16

/* usage: epoll <file1> [file2] [file3] ... */

/*********************************函数*****************************************/

int main(int argc, char *argv[])
{
    int ret;
	int result;
    int epoll_fd;
    int tmp_fd;
    int len;
    int i;
    char buf[DATA_MAX_LEN];
    struct epoll_event ep_event[EPOLL_MAX_EVENTS];
    struct epoll_event *tmp_ep_event = NULL;

    if (argc < 2) {
        printf("Usage: %s <file1> [file2] [file3] ...\n", argv[0]);
        return -1;
    }

    /* epoll_create */
    epoll_fd = epoll_create(1);
    tmp_ep_event = (struct epoll_event *)malloc((argc - 1) * \
                    sizeof(struct epoll_event));
    if (!tmp_ep_event) {
        printf("malloc struct epoll_event failed!\n");
        return -1;
    }
    memset(tmp_ep_event, 0, sizeof(tmp_ep_event));
    /* for each file:
     * open it
     * add it to epoll: epoll_ctl(...EPOLL_CTL_ADD...)
     */
    for (i = 1; i < argc; i++) {
        //tmp_fd = open(argv[i], O_RDONLY|O_NONBLOCK);
        tmp_fd = open(argv[i], O_RDWR);
        tmp_ep_event->events = EPOLLIN;
        tmp_ep_event->data.fd = tmp_fd;

        ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tmp_fd, tmp_ep_event);
        if (ret) {
            printf("fialed to add\n");
        }

        tmp_ep_event++;
    }

    /* epoll_wait */
    while (1) {
        result = epoll_wait(epoll_fd, ep_event, EPOLL_MAX_EVENTS, -1);
        for (i = 0; i < result; i++) {
            printf("Reason: 0x%x\n", ep_event[i].events);
            len = read(ep_event[i].data.fd, buf, DATA_MAX_LEN);
            buf[len] = '\0';
            printf("get data: %s\n", buf);
            //sleep(3);
        }
    }

    return 0;
}

