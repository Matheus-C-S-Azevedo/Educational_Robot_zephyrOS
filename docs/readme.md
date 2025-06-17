codigo criado por matheus costa

Referencias:
como usar o ADC driver: https://docs.zephyrproject.org/latest/samples/drivers/adc/adc_dt/README.html#adc_dt


fluxo do código main.c

// --- CONFIGURAÇÕES ---
#define MQTT_BROKER_ADDR "192.168.0.152"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC       "botao/estado"
#define MQTT_CLIENT_ID   "esp32-zephyr-client"

// --- GPIOs ---
#define LED_NODE    DT_ALIAS(led0)
#define BUTTON_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);




/*=============================================================== MQQTT =================================================*/
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
