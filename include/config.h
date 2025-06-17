#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "LAB_I4.0"
#define WIFI_PSK "L4B0@2025#"

#define MQTT_BROKER_ADDR "192.168.0.102"
#define MQTT_BROKER_PORT 1883

#define MQTT_CLIENT_ID "zephyr_client"
#define MQTT_TOPIC_DEFAULT "zephyr/test"
#define MQTT_QOS MQTT_QOS_0_AT_MOST_ONCE

#endif

