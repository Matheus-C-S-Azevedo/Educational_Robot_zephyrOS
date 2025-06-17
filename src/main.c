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

LOG_MODULE_REGISTER(main_app, LOG_LEVEL_INF);
// cria a thread para fazer as conexões wifi e mqtt
//K_THREAD_DEFINE(connection_watchdog_id, 1024, connection_watchdog, NULL, NULL, NULL, 1, 0, 0);

void main(void)
{
    // inicia a thread de conexão Wi-Fi + MQTT
    connection_manager_start();

    while (1) {
        // aqui você pode eventualmente publicar mensagens MQTT
        k_sleep(K_SECONDS(1));
    }
}