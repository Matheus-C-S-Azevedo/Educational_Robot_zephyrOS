#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stddef.h> // para NULL
#include "wifi_connect.h"
#include "broker_connect.h"

LOG_MODULE_REGISTER(connection_manager, LOG_LEVEL_INF);

#define STACK_SIZE 2048
#define PRIORITY 1
K_THREAD_STACK_DEFINE(conn_stack, STACK_SIZE);
static struct k_thread conn_thread;

#define MQTT_STACK_SIZE 6144
#define MQTT_PRIORITY 2
K_THREAD_STACK_DEFINE(mqtt_stack, MQTT_STACK_SIZE);
static struct k_thread mqtt_thread;

struct k_thread *mqtt_thread_id = NULL;

void mqtt_connection_thread(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);
    LOG_INF("Thread MQTT iniciada");
    connect_to_mqtt();
}

void connection_watchdog(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);

    while (1) {
        if (!wifi_conectado) {
            LOG_INF("Tentando se reconectar ao wifi ...");
            connect_to_wifi();
        } else if (!mqtt_conectado && mqtt_thread_id == NULL) {
            LOG_INF("Tentando se reconectar ao mqtt (em thread separada)");
            mqtt_thread_id = k_thread_create(&mqtt_thread, mqtt_stack, MQTT_STACK_SIZE,
                                            mqtt_connection_thread, NULL, NULL, NULL,
                                            MQTT_PRIORITY, 0, K_NO_WAIT);
        }

        k_sleep(K_SECONDS(10));
    }
}

void connection_manager_start(void)
{
    LOG_INF("Iniciando Connection Manager...");
    k_thread_create(&conn_thread, conn_stack, STACK_SIZE,
                    connection_watchdog, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
}
