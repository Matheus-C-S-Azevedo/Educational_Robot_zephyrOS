#include <zephyr/net/mqtt.h>
#include <zephyr/logging/log.h>
#include "mqtt_publish.h"
#include "config.h"
#include "broker_connect.h"
#include <zephyr/random/random.h>


LOG_MODULE_REGISTER(mqtt_publish, LOG_LEVEL_INF);

// Extern para acessar a instância global
extern struct mqtt_client mqtt_global_client;
extern volatile bool mqtt_conectado;

int mqtt_publish_data(const char *topic, const char *payload)
{
    if (!mqtt_conectado) {
        LOG_WRN("Abortando publish: MQTT não está conectado.");
        return -ENOTCONN;
    }

    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)topic;
    param.message.topic.topic.size = strlen(topic);
    param.message.payload.data = (uint8_t *)payload;
    param.message.payload.len = strlen(payload);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;

    int rc = mqtt_publish(&mqtt_global_client, &param);
    if (rc != 0) {
        LOG_ERR("Erro ao publicar: %d", rc);
    } else {
        LOG_INF("Mensagem publicada: %s", payload);
    }

    return 0;
}
