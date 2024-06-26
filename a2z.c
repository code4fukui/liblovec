#include "liblove.c"
#include <time.h>

int main() {
  Keyboard kbd;
  Keyboard_(&kbd);
  setbuf(stdout, NULL);

  time_t t = time(NULL);

  int c = 'a';
  while (c <= 'z') {
    int k = Keyboard_inkey(&kbd);
    if (k == 0) {
      continue;
    }
    if (k != c) {
      putchar('\a');
      continue;
    }
    putchar(c);
    c = c + 1;
  }

  printf("\n%d\n", (int)(time(NULL) - t));

  Keyboard__(&kbd);
  return 0;
}
