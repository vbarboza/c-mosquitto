#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "config.h"
#include "pub.h"


void publish_callback(struct mosquitto *mosq, void *data, int mid) {
  p_pub("%s: sent message %d\n", ((client_data_t *)data)->client_id, mid);
}


struct mosquitto *create_publisher(const char *id) {
  int rc = 0;
  struct mosquitto *mosq;

  client_data_t *data;
  data = (client_data_t *)malloc(sizeof(client_data_t));
  strcpy(data->client_id, id);

  // Create a Mosquitto client
  mosq = mosquitto_new(id, true, data);
  if (!mosq) {
    p_error("failed to create publisher %s (%d - %s)\n", id, errno, strerror(errno));
    exit(-1);
  }

  // Set a publish and log callback
  mosquitto_publish_callback_set(mosq, publish_callback);
  mosquitto_log_callback_set(mosq, log_callback);

  // Connect to a MQTT broker
  rc = mosquitto_connect(mosq, MOSQ_HOST, MOSQ_PORT, MOSQ_KEEPALIVE);
  if (rc) {
    p_error("failed to connect (%d - %s)\n", errno, strerror(errno));
    exit(rc);
  }

  // Start a new thread for this client
  rc = mosquitto_loop_start(mosq);
  if (rc) {
    p_error("failed to start %s loop (%d - %s)\n", "m0", errno, strerror(errno));
    exit(rc);
  }
  
  return mosq;
}

void destroy_publisher(struct mosquitto *mosq) {
  int rc = 0;
  
  mosquitto_disconnect(mosq);
  if (rc) {
    p_error("failed to disconnect (%d - %s)\n", errno, strerror(errno));
    exit(rc);
  }

  mosquitto_loop_stop(mosq, false);
  if (rc) {
    p_error("failed to stop %s loop (%d - %s)\n", "m0", errno, strerror(errno));
    exit(rc);
  }

  mosquitto_destroy(mosq);
}

void publish(struct mosquitto *mosq, const char *topic, const char *msg) {
  int rc = 0;
  // TO DO: handle QoS, retaining and errors
  rc = mosquitto_publish(mosq, NULL, topic, strlen(msg), (const void *)msg, 1, 0);
  if (rc) {
    p_error("failed to publish (%d - %s)\n", errno, strerror(errno));
  }
}
