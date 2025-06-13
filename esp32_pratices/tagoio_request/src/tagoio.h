#pragma once

#include <zephyr/net/http/client.h>

#define TAGOIO_PAYLOAD_SIZE_BYTES 512
#define TAGOIO_RECV_BUFFER_SIZE_BYTES 512
#define TAGOIO_DEVICE_ID_SIZE_BYTES 16





struct tagoio {
    char tagoio_payload[TAGOIO_PAYLOAD_SIZE_BYTES];
    char tagoio_recv[TAGOIO_RECV_BUFFER_SIZE_BYTES];
    char device_id[TAGOIO_DEVICE_ID_SIZE_BYTES];
    int socket;

};


int tagoio_connect(struct tagoio *info);
int tagio_send_payload(struct tagoio *info,http_response_cb_t resp_cb);