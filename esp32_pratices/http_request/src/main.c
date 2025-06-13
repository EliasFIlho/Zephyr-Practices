/*
 *
 * Init Wifi and send a HTTP GET request to google
 *
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/dhcpv4_server.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/http/client.h>
#include <string.h>
#include "wifi.h"

LOG_MODULE_REGISTER(MAIN);

/* STA Mode Configuration */
#define WIFI_SSID "LINKCE- 2G"
#define WIFI_PSK "20122000"

#define HTTP_REQUEST_HOST "google.com"
#define HTTP_REQUEST_PORT "80"

#define HTTP_RECV_BUF_LEN 512
#define HTTP_REQUEST_TIMEOUT 3000

// Globals
static uint8_t recv_buf[HTTP_RECV_BUF_LEN];

// HTTP request callback
static void response_callback(struct http_response *resp,
                              enum http_final_call final_data,
                              void *user_data)
{
    char temp_buf[HTTP_RECV_BUF_LEN + 1];

    // Check if we still have more data to receive
    if (final_data == HTTP_DATA_MORE)
    {
        printk("Partial data received (%d bytes)\r\n", resp->data_len);
    }
    else if (final_data == HTTP_DATA_FINAL)
    {
        printk("All data received (%d bytes)\r\n", resp->data_len);
    }

    // Print the received data (up to data_len bytes)
    memcpy(temp_buf, resp->recv_buf, resp->data_len);
    temp_buf[resp->data_len] = '\0';
    printk("Received data:\r\n%s\r\n", temp_buf);
}

static void print_addrinfo(struct zsock_addrinfo **results)
{
    char ipv4[INET_ADDRSTRLEN];
    char ipv6[INET6_ADDRSTRLEN];
    struct sockaddr_in *sa;
    struct sockaddr_in6 *sa6;
    struct zsock_addrinfo *rp;

    // Iterate through the results
    for (rp = *results; rp != NULL; rp = rp->ai_next)
    {

        // Print IPv4 address
        if (rp->ai_addr->sa_family == AF_INET)
        {
            sa = (struct sockaddr_in *)rp->ai_addr;
            zsock_inet_ntop(AF_INET, &sa->sin_addr, ipv4, INET_ADDRSTRLEN);
            printk("IPv4: %s\r\n", ipv4);
        }

        // Print IPv6 address
        if (rp->ai_addr->sa_family == AF_INET6)
        {
            sa6 = (struct sockaddr_in6 *)rp->ai_addr;
            zsock_inet_ntop(AF_INET6, &sa6->sin6_addr, ipv6, INET6_ADDRSTRLEN);
            printk("IPv6: %s\r\n", ipv6);
        }
    }
}

int main(void)
{
    k_sleep(K_SECONDS(3));

    // Init TCP socket
    struct zsock_addrinfo hints = {0};
    struct zsock_addrinfo *res;
    int sock;
    int ret;

    struct http_request req = {0};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    printk("Initing wifi...\r\n");
    wifi_init();
    connect_to_wifi(WIFI_SSID, WIFI_PSK);

    wait_wifi_connect();

    ret = zsock_getaddrinfo(HTTP_REQUEST_HOST, HTTP_REQUEST_PORT, &hints, &res);
    if (ret != 0)
    {
        printk("Error[%d]: Could not get addr info from DNS host [%s] - %s\n\r", errno, HTTP_REQUEST_HOST, strerror(errno));
        wifi_disconnect();
        return 0;
    }
    else
    {
        printk("Socket addr info collected!!\n\r");
        print_addrinfo(&res);
    }

    sock = zsock_socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    memset(&req, 0, sizeof(req));
    req.method = HTTP_GET;
    req.url = "/";
    req.protocol = "HTTP/1.1";
    req.port = HTTP_REQUEST_PORT;
    req.host = HTTP_REQUEST_HOST;
    req.response = response_callback;
    req.recv_buf = recv_buf;
    req.recv_buf_len = sizeof(recv_buf);

    ret = zsock_connect(sock, res->ai_addr, res->ai_addrlen);

    if (ret < 0)
    {
        printk("Error[%d]: Socket could not connect - %s\n\r", errno, strerror(errno));
    }
    else
    {
        printk("Socket connected!!\n\r");
    }
    while (1)
    {

        ret = http_client_req(sock, &req, HTTP_REQUEST_TIMEOUT, NULL);
        if (ret >= 0)
        {
            printk("Amount of Sent data: %d\n\r", ret);
        }
        else
        {
            printk("Error to send data: %d\n\r", ret);
        }

        k_sleep(K_SECONDS(5));
    }

    zsock_close(sock);
    wifi_disconnect();

    return 0;
}
