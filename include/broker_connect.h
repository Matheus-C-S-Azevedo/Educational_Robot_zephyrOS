#ifndef BROKER_CONNECT_H
#define BROKER_CONNECT_H

#include <zephyr/net/mqtt.h>

int connect_to_mqtt(struct mqtt_client *client, struct sockaddr_storage *broker);
extern volatile bool mqtt_conectado;

#endif // BROKER_CONNECT_H
