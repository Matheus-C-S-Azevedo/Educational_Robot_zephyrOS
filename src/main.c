#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/dhcpv4_server.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <string.h>

#include "wifi_connect.h"
#include "config.h"
#include "connection_manager.h"
#include "mqtt_publish.h"

LOG_MODULE_REGISTER(main_app, LOG_LEVEL_INF);
extern volatile bool mqtt_conectado;


int main(void)
{
    connection_manager_start();

    while (!mqtt_conectado) {
        LOG_INF("Aguardando conexão MQTT...");
        k_sleep(K_SECONDS(1));
    }

    while (1) {
        k_sleep(K_SECONDS(10));

        if (mqtt_conectado) {
            int ret = mqtt_publish_data("zephyr/test", "olá do main!");
            if (ret != 0) {
                LOG_ERR("Falha ao publicar MQTT: %d", ret);
            }
        } else {
            LOG_WRN("MQTT não conectado, pulando publicação.");
        }
    }

    return 0;
}
