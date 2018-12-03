#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

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
