#pragma once

void wifi_init(void);
int connect_to_wifi(char *ssid, char *psk);
void wait_wifi_connect(void);
int wifi_disconnect(void);
void wifi_wait_for_ip_addr(void);