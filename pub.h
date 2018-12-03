#ifndef _PUB_H
#define _PUB_H

#include <mosquitto.h>

struct mosquitto *create_publisher(const char *id);

void publish(struct mosquitto *mosq, const char *topic, const char *msg);

void destroy_publisher(struct mosquitto *mosq);

#endif
