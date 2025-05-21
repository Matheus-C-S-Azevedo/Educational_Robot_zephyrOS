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