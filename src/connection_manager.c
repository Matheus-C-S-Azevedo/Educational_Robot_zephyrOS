#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "wifi_connect.h"

LOG_MODULE_REGISTER(connection_manager, LOG_LEVEL_INF);

void connection_watchdog(void *p1, void *p2, void *p3)
{

    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);
    
    while (1) {
        // Cuida da reconexão do wifi
        if (!wifi_conectado) {
            LOG_INF("Tentando se reconectar ao wifi ...");
            connect_to_wifi();
        }

        k_sleep(K_SECONDS(10));
    }
}