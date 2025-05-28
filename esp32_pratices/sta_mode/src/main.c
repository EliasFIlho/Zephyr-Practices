/*
 *
 * Init Wifi and send a HTTP GET request to google
 *
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/dhcpv4_server.h>
#include "wifi.h"
#include <zephyr/net/socket.h>
#include <string.h>

LOG_MODULE_REGISTER(MAIN);

/* STA Mode Configuration */
#define WIFI_SSID "LINKCE- 2G" /* Replace `SSID` with WiFi ssid. */
#define WIFI_PSK "20122000"    /* Replace `PASSWORD` with Router password. */

#define HTTP_REQUEST_HOST "google.com"
#define HTTP_REQUEST_URL "/"

char response[512];

int main(void)
{
    k_sleep(K_SECONDS(5));

    // Init and Connect to wifi
    wifi_init();
    connect_to_wifi(WIFI_SSID, WIFI_PSK);

    // Wait until connect (maybe use a semaphore to block)
    wait_wifi_connect();

    // Init TCP socket
    struct zsock_addrinfo hints = {0};
    struct zsock_addrinfo *res;
    int sock;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int ret;
    ret = zsock_getaddrinfo(HTTP_REQUEST_HOST, "80", &hints, &res);
    if (ret != 0)
    {
        printk("Error[%d]: Could not get addr info from DNS host [%s] - %s\n\r", errno, HTTP_REQUEST_HOST, strerror(errno));
        return 0;
    }
    else
    {
        printk("Sucess get addr info from DNS host: %s\n\r", HTTP_REQUEST_HOST);
    }

    sock = zsock_socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sock < 0)
    {
        printk("Error[%d]: Cant create socket - %s\n\r", errno, strerror(errno));
        return 0;
    }
    else
    {
        printk("Socket created!!\n\r");
    }

    ret = zsock_connect(sock, res->ai_addr, res->ai_addrlen);

    if (ret < 0)
    {
        printk("Error[%d]: Socket could not connect - %s\n\r", errno, strerror(errno));
        return 0;
    }
    else
    {
        printk("Socket connected!!\n\r");
    }

    char request_buffer[512];

    snprintf(request_buffer, sizeof(request_buffer),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n"
             "\r\n",
             HTTP_REQUEST_URL, HTTP_REQUEST_HOST);

    ret = zsock_send(sock,request_buffer,sizeof(request_buffer),0);

    if (ret < 0)
    {
        printk("Error[%d]: Socket could not send data - %s\n\r", errno, strerror(errno));
        return 0;
    }
    else
    {
        printk("Socket sended data!!\n\r");
    }

        

    while (1)
    {
        k_msleep(100);
    }

    zsock_close(sock);

    return 0;
}
