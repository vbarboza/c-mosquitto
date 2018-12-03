#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <mosquitto.h>

// #define DEBUG

#define MOSQ_HOST      ((char *) "localhost")
#define MOSQ_PORT      ((int) 1883)
#define MOSQ_KEEPALIVE ((int) 60)

#define p_debug(fmt, ...)						\
  fprintf(stderr, "[%10ld] DEBUG: " fmt, time(NULL), ##__VA_ARGS__);
#define p_info(fmt, ...)						\
  fprintf(stderr, "[%10ld] INFO: " fmt, time(NULL), ##__VA_ARGS__);
#define p_notice(fmt, ...)						\
  fprintf(stderr, "[%10ld] NOTICE: " fmt, time(NULL), ##__VA_ARGS__);
#define p_warning(fmt, ...)						\
  fprintf(stderr, "[%10ld] WARNING: " fmt, time(NULL), ##__VA_ARGS__);
#define p_error(fmt, ...)						\
  fprintf(stderr, "[%10ld] ERROR: " fmt, time(NULL), ##__VA_ARGS__);

#define p_pub(fmt, ...)						\
  printf("[%10ld] publisher: " fmt, time(NULL), ##__VA_ARGS__)
#define p_sub(fmt, ...)						\
  printf("[%10ld] subscriber: " fmt, time(NULL), ##__VA_ARGS__)

// TO DO: use mosquitto_client_id() from mosquitto_broker.h
typedef struct client_data {
  char client_id[32];
} client_data_t;

void log_callback(struct mosquitto *mosq, void *data, int level, const char *str) {
  switch (level) {
#ifdef DEBUG
  case MOSQ_LOG_DEBUG:
    p_debug("%s: %s\n", ((client_data_t *)data)->client_id, str)
      break;
#endif
  case MOSQ_LOG_INFO:
    p_info("%s: %s\n", ((client_data_t *)data)->client_id, str)
      break;
  case MOSQ_LOG_NOTICE:
    p_notice("%s: %s\n", ((client_data_t *)data)->client_id, str)
      break;
  case MOSQ_LOG_WARNING:
    p_warning("%s: %s\n", ((client_data_t *)data)->client_id, str)
      break;
  case MOSQ_LOG_ERR:
    p_error("%s: %s\n", ((client_data_t *)data)->client_id, str)
      break;
  default:
    break;
  }
}

void publish_callback(struct mosquitto *mosq, void *data, int mid) {
  p_pub("%s: sent message %d\n", ((client_data_t *)data)->client_id, mid);
}

void message_callback(struct mosquitto *mosq, void *data,
		      const struct mosquitto_message *message) {
  p_sub("%s: received %s message %d (\"%s\")\n",
	message->topic,
	(const char *)(message->retain ? "(retain'ed)":""),
	message->mid,
	(const char *)message->payload);
}

struct mosquitto *mosquitto_publisher(const char *id) {
  struct mosquitto *mosq;

  client_data_t *data;
  data = (client_data_t *)malloc(sizeof(client_data_t));
  strcpy(data->client_id, id);
  
  mosq = mosquitto_new(id, true, data);
  if (!mosq) {
    p_error("failed to create publisher %s (%d - %s)\n", id, errno, strerror(errno));
    exit(-1);
  }
  
  mosquitto_publish_callback_set(mosq, publish_callback);
  mosquitto_log_callback_set(mosq, log_callback);
  
  return mosq;
}

int publish(struct mosquitto *mosq, const char *topic, const char *msg) {
  // TO DO: handle QoS, retaining and errors
  return mosquitto_publish(mosq, NULL, topic, strlen(msg), (const void *)msg, 1, 0);
}


int main(int argc, char *argv[]) {
  int i;
  int rc;
  struct mosquitto *mosq = NULL;

  mosquitto_lib_init();
  
  mosq = mosquitto_publisher("m0");

  rc = mosquitto_connect(mosq, MOSQ_HOST, MOSQ_PORT, MOSQ_KEEPALIVE);
  if (rc) {
    p_error("failed to connect (%d - %s)\n", rc, strerror(errno));
    exit(-1);
  }

  rc = mosquitto_loop_start(mosq);
  if (rc) {
    p_error("failed to start %s loop (%d - %s)\n", "m0", rc, strerror(errno));
    exit(-1);
  }
  
  for (i = 0; i < 10; i++) {
    rc = publish(mosq, "topic", "0");
    if (rc) {
      p_error("failed to publish (%d - %s)\n", rc, strerror(errno));
    }    
    sleep(1);
  }

  mosquitto_disconnect(mosq);
  mosquitto_loop_stop(mosq, false);
  mosquitto_destroy(mosq);
  mosquitto_lib_cleanup();
  return 0;
}
