#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>
#include <string.h>
#include "config.h"
#include "broker_connect.h"

LOG_MODULE_REGISTER(broker_connect, LOG_LEVEL_INF);

volatile bool mqtt_conectado = false;
struct mqtt_client mqtt_global_client;
struct sockaddr_storage mqtt_broker;


static void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result == 0) {
            LOG_INF("MQTT conectado com sucesso.");
            mqtt_conectado = true;
        } else {
            LOG_ERR("Falha na conexão MQTT: %d", evt->result);
            mqtt_conectado = false;
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_WRN("Desconectado do broker MQTT.");
        mqtt_conectado = false;
        break;
    default:
        break;
    }
}

int connect_to_mqtt(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&mqtt_broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&mqtt_global_client);

    mqtt_global_client.broker = &mqtt_broker;
    mqtt_global_client.evt_cb = mqtt_evt_handler;
    mqtt_global_client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    mqtt_global_client.client_id.size = strlen(MQTT_CLIENT_ID);
    mqtt_global_client.protocol_version = MQTT_VERSION_3_1_1;
    mqtt_global_client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    int ret = mqtt_connect(&mqtt_global_client);
    if (ret != 0) {
        LOG_ERR("Erro ao tentar conectar ao broker MQTT: %d", ret);
        mqtt_conectado = false;
        return ret;
    }

    return 0;
}