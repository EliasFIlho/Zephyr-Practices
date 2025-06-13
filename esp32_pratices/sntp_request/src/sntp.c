#include "sntp.h"
#include <zephyr/net/socket.h>
#include <zephyr/net/sntp.h>
#include <time.h>
#include <arpa/inet.h>

#define SNTP_HOST "a.st1.ntp.br"

int get_current_time(struct sntp_time *s_time)
{
    int ret = sntp_simple(SNTP_HOST, 4000, s_time);

    if (ret != 0)
    {
        printk("Error to do SNTP request\r\n");
    }
    else
    {
        printk("SNTP Time: %llu\r\n", s_time->seconds);
    }

    return ret;
}
