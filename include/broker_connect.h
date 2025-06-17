#ifndef BROKER_CONNECT_H
#define BROKER_CONNECT_H

#include <zephyr/net/mqtt.h>

int connect_to_mqtt();
extern volatile bool mqtt_conectado;
extern struct k_thread *mqtt_thread_id;

#endif // BROKER_CONNECT_H
