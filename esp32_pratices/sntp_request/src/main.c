/*
 *
 * Init Wifi and send a HTTP GET request to google
 *
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>
#include "wifi.h"
#include "sntp.h"

LOG_MODULE_REGISTER(MAIN);

/* STA Mode Configuration */
#define WIFI_SSID "LINKCE- 2G"
#define WIFI_PSK "20122000"

int main(void)
{

    k_sleep(K_SECONDS(3));
    int ret;
    printk("Initing wifi...\r\n");
    wifi_init();
    connect_to_wifi(WIFI_SSID, WIFI_PSK);
    wait_wifi_connect();
    wifi_wait_for_ip_addr();

    struct sntp_time s_time;
    ret = get_current_time(&s_time);

   

    k_msleep(3000);
    wifi_disconnect();

    return 0;
}
