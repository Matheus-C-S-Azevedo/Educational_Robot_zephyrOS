#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "iPhone de Matheus"
#define WIFI_PSK "matheus123"

#define MQTT_BROKER_ADDR "172.20.10.11"
#define MQTT_BROKER_PORT 1883

#define MQTT_CLIENT_ID "zephyr_client"
#define MQTT_TOPIC_DEFAULT "zephyr/test"
#define MQTT_QOS MQTT_QOS_0_AT_MOST_ONCE

#endif

