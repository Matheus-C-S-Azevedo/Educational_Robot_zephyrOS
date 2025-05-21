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

LOG_MODULE_REGISTER(main_app, LOG_LEVEL_INF);

#define MQTT_BROKER_ADDR "192.168.0.152"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32-zephyr"
#define MQTT_TOPIC "ldr/valor"  // ou outro de sua escolha


#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256

static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t tx_buffer[TX_BUFFER_SIZE];

static struct mqtt_client client;
static uint8_t rx_buffer[256];
static uint8_t tx_buffer[256];
static struct sockaddr_storage broker;

static void mqtt_configurar_cliente(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = NULL; // pode ser substituído por um handler
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
}

static int mqtt_conectar(void)
{
    int rc = mqtt_connect(&client);
    if (rc != 0) {
        LOG_ERR("❌ mqtt_connect falhou: %d", rc);
    } else {
        LOG_INF("✅ Conectado ao broker MQTT em %s:%d", MQTT_BROKER_ADDR, MQTT_BROKER_PORT);
    }
    return rc;
}

static void mqtt_publicar(const char *mensagem)
{
    struct mqtt_publish_param pub_param;

    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    pub_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    pub_param.message.payload.data = (uint8_t *)mensagem;
    pub_param.message.payload.len = strlen(mensagem);
    pub_param.message_id = 0;
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    int rc = mqtt_publish(&client, &pub_param);
    if (rc) {
        LOG_ERR("❌ Falha ao publicar mensagem (%d)", rc);
    } else {
        LOG_INF("📤 Mensagem publicada: %s", mensagem);
    }
}

void main(void)
{
    k_sleep(K_SECONDS(5));

    connect_to_wifi();

    k_sleep(K_SECONDS(10));

    mqtt_configurar_cliente();

    if (mqtt_conectar() != 0) {
        return;
    }

    k_sleep(K_SECONDS(1));  // Espera conexão estabilizar (não usamos client.state)

    mqtt_publicar("1");

    while (1) {
        mqtt_input(&client);
        mqtt_live(&client);
        k_sleep(K_SECONDS(1));
    }
}