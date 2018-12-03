#ifndef _COMMON_H
#define _COMMON_H

#include <mosquitto.h>

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
  char client_id[36];
} client_data_t;

void log_callback(struct mosquitto *mosq, void *data, int level, const char *str);

#endif
