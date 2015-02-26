#include <pebble.h>
#include "window.h"

int main(void) {
  show_window();
  app_event_loop();
  hide_window();
}
