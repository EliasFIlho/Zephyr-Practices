/*
 *
 * Init Wifi and send a HTTP GET request to google
 *
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/http/client.h>
#include <string.h>
#include "wifi.h"
#include "tagoio.h"

LOG_MODULE_REGISTER(MAIN);

/* STA Mode Configuration */
#define WIFI_SSID ""
#define WIFI_PSK ""


#define HTTP_RECV_BUF_LEN 512


// HTTP request callback
static void response_callback(struct http_response *resp,
                              enum http_final_call final_data,
                              void *user_data)
{
    char temp_buf[HTTP_RECV_BUF_LEN + 1];

    if (final_data == HTTP_DATA_MORE)
    {
        printk("Partial data received (%d bytes)\r\n", resp->data_len);
    }
    else if (final_data == HTTP_DATA_FINAL)
    {
        printk("All data received (%d bytes)\r\n", resp->data_len);
    }

    memcpy(temp_buf, resp->recv_buf, resp->data_len);
    temp_buf[resp->data_len] = '\0';
    printk("Received data:\r\n%s\r\n", temp_buf);
}

// Fake data example
static void create_data_payload(char *buffer)
{

    int rpm = 1495;
    int target = 1500;
    int error = 5;

    snprintf(buffer,174,
             "["
             "{\"variable\":\"Revolutions per minute\",\"value\":\"%d\",\"unit\":\"RPM\"},"
             "{\"variable\":\"Target\",\"value\":\"%d\",\"unit\":\"RPM\"},"
             "{\"variable\":\"Error\",\"value\":\"%d\",\"unit\":\"RPM\"}"
             "]",
             rpm, target, error);
}

int main(void)
{

    k_sleep(K_SECONDS(3));

    printk("Initing wifi...\r\n");
    wifi_init();
    connect_to_wifi(WIFI_SSID, WIFI_PSK);
    wait_wifi_connect();
    wifi_wait_for_ip_addr();


    struct tagoio data;
    memset(&data, 0, sizeof(data));

    int connect_try = 15;
    while (connect_try--)
    {
        if (tagoio_connect(&data) < 0)
        {
            printk("Error to connect to tagoio - trys count [%d]\r\n",connect_try);
        }else{
            printk("Connected\r\n");
            break;
        }
        k_msleep(250);
    }
    if(connect_try == 0){
        printk("Error to connect to tagoio");
    }
    create_data_payload(data.tagoio_payload);
    printk("Payload: \r\n%s\r\n",data.tagoio_payload);
    
    tagio_send_payload(&data,response_callback);

    k_msleep(3000);
    wifi_disconnect();

    return 0;
}
