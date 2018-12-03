#include <stdio.h>
#include <unistd.h>

#include "pub.h"

/*
void message_callback(struct mosquitto *mosq, void *data,
		      const struct mosquitto_message *message) {
  p_sub("%s: received %s message %d (\"%s\")\n",
	message->topic,
	(const char *)(message->retain ? "(retain'ed)":""),
	message->mid,
	(const char *)message->payload);
}
*/

int main(int argc, char *argv[]) {
  int i;
  int rc;
  struct mosquitto *mosq = NULL;

  mosquitto_lib_init();
  mosq = create_publisher("m0");

  for (i = 0; i < 10; i++) {
    publish(mosq, "topic", "0");
    sleep(1);
  }
  
  destroy_publisher(mosq);
  mosquitto_lib_cleanup();
  return 0;
}
