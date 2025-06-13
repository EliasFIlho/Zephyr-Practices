#pragma once

#include <zephyr/net/sntp.h>



//int get_current_time(struct ntp_time *s_time);
int get_current_time(struct sntp_time *s_time);
