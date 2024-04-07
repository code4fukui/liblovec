#include "liblove.c"
#include <time.h>

int main() {
  Keyboard kbd;
  Keyboard_(&kbd);
  setbuf(stdout, NULL);

  time_t t;
  int c, k;

  //-- like a BASIC
  t = time(NULL);                               // CLT
  c = 'a';                                      // C=ASC("A")
  Line30:
  k = Keyboard_inkey(&kbd);                     // K=INKEY()
  if (k == 0){ goto Line30; }                   // IF K=0 GOTO 30
  if (k != c){ putchar('\a'); goto Line30; }    // IF K!=C BEEP:GOTO 30
  putchar(c);                                   // ?CHR$(C);
  c = c + 1;                                    // C=C+1
  if (c <= 'z') goto Line30;                    // IF C<=ASC("Z") GOTO 30
  t = time(NULL)-t; printf("\n%ld\n", t);       // T=TICK():?:?T/60;".";T/6%60
  //--

  Keyboard__(&kbd);
  return 0;
}
