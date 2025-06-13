#include "tagoio.h"

#include <zephyr/net/socket.h>
#include <zephyr/net/http/client.h>


#define TAGOIO_HOSTNAME "api.tago.io"
#define TAGOIO_ENDPOINT "/"
#define TAGOIO_PORT "80"

#define HTTP_REQUEST_TIMEOUT 3000

static const char *tagoio_http_headers[] = {
	"Device-Token: \"  \"\r\n",
	"Content-Type: application/json\r\n",
	"_ssl: false\r\n",
	NULL
};

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

int tagoio_connect(struct tagoio *info)
{
    int ret;
    // Init TCP socket
    struct zsock_addrinfo hints = {0};
    struct zsock_addrinfo *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    ret = zsock_getaddrinfo(TAGOIO_HOSTNAME, TAGOIO_PORT, &hints, &res);
    if (ret != 0)
    {   
        printk("Ret value: %d",ret);
        printk("Error[%d]: Could not get addr info from DNS host [%s] - %s\r\n", errno, TAGOIO_HOSTNAME, strerror(errno));
        //wifi_disconnect();
        return -1;
    }
    else
    {
        printk("Socket addr info collected!!\n\r");
        print_addrinfo(&res);
    }
    info->socket = zsock_socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    ret = zsock_connect(info->socket, res->ai_addr, res->ai_addrlen);

    if (ret < 0)
    {
        printk("Error[%d]: Socket could not connect - %s\n\r", errno, strerror(errno));
    }
    else
    {
        printk("Socket connected!!\n\r");
    }
    return ret;
}

int tagio_send_payload(struct tagoio *info,http_response_cb_t resp_cb)
{
    struct http_request req;
    memset(&req, 0, sizeof(req));


    req.method = HTTP_POST;
    req.url = TAGOIO_ENDPOINT;
    req.protocol = "HTTP/1.1";
    req.header_fields = tagoio_http_headers;
    req.port = TAGOIO_PORT;
    req.host = TAGOIO_HOSTNAME;
    req.response = resp_cb;
    req.recv_buf = info->tagoio_recv;
    req.recv_buf_len = sizeof(info->tagoio_recv);
    req.payload = info->tagoio_payload;
    req.payload_len = sizeof(info->tagoio_payload);

    return http_client_req(info->socket,&req,HTTP_REQUEST_TIMEOUT,info);



}